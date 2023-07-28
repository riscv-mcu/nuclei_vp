/*
 * BaseMemory.h
 *
 * Copyright (C) 2015, GreenSocs Ltd.
 *
 * Developed by Konrad Frederic <fred.konrad@greensocs.com>
 *              Guillaume Delbergue <guillaume.delbergue@greensocs.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses>.
 *
 * Linking GreenSocs code, statically or dynamically with other modules
 * is making a combined work based on GreenSocs code. Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * In addition, as a special exception, the copyright holders, GreenSocs
 * Ltd, give you permission to combine GreenSocs code with free software
 * programs or libraries that are released under the GNU LGPL, under the
 * OSCI license, under the OCP TLM Kit Research License Agreement or
 * under the OVP evaluation license.You may copy and distribute such a
 * system following the terms of the GNU GPL and the licenses of the
 * other code concerned.
 *
 * Note that people who make modified versions of GreenSocs code are not
 * obligated to grant this special exception for their modified versions;
 * it is their choice whether to do so. The GNU General Public License
 * gives permission to release a modified version without this exception;
 * this exception also makes it possible to release a modified version
 * which carries forward this exception.
 *
 */

/*
 * @file BaseMemory.h
 * @author Guillaume Delbergue <guillaume.delbergue@greensocs.com>
 * @author Frederic Konrad <fred.konrad@greensocs.com>
 * @date May, 2015
 * @copyright Copyright (C) 2015, GreenSocs Ltd.
 *
 * @brief Base memory for SystemC using GreenLib.
 * @details You shouldn't directly instantiate this class.
 */

#ifndef BASE_MEMORY_INCLUSION_PROTECTOR
#error __FILE__ "must not be included directly."
#endif /* BASE_MEMORY_INCLUSION_PROTECTOR */

#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

//#define MEMORY_DEBUG
#ifdef MEMORY_DEBUG
#define DCOUT(var) std::cout << "BaseMemory: " << sc_core::sc_time_stamp()     \
                             << " :" << var << std::endl;
#else
#define DCOUT(var)
#endif

#include <stdint.h>
#include <sys/mman.h>

class BaseMemory
{
public:
    BaseMemory()
    {
    }

    ~BaseMemory()
    {
        if (m_ptr)
        {
            munmap(m_ptr, m_size * 1024);
        }
    }

    void allocate_memory(size_t size, bool ro)
    {
        m_size = size;
        m_ro = ro;
        m_ptr = mmap(0, m_size * 1024, PROT_READ | PROT_WRITE,
                                       MAP_ANONYMOUS | MAP_PRIVATE,
                                       -1, 0);

        if (!m_ptr)
        {
            std::cout << "BaseMemory: error: can't create memory." << std::endl;
            sc_core::sc_stop();
            return;
        }
    }

    void setReadOnly(bool read_only)
    {
        m_ro = read_only;
    }

    unsigned int dbg_transport(tlm::tlm_generic_payload& payload,
		               uint32_t offset)
    {
        tlm::tlm_command command = payload.get_command();
        unsigned char *dataPtr = payload.get_data_ptr();
        unsigned int length = payload.get_data_length();

	switch (command)
	{
	    case tlm::TLM_READ_COMMAND:
	      memcpy(dataPtr, &(((uint8_t *)m_ptr)[offset]), length);
            break;
            case tlm::TLM_WRITE_COMMAND:
	      memcpy(&(((uint8_t *)m_ptr)[offset]), dataPtr, length);
            break;
            default:
	    break;
	}

        payload.set_response_status(tlm::TLM_OK_RESPONSE);
        return length;
    }

    void b_transact(gs::gp::GenericSlavePort<32>::accessHandle t,
                    uint32_t offset)
    {
        size_t size = t->getMBurstLength();

        if (offset + size > m_size * 1024)
        {
            std::cout << "error out of bound memory access.." << std::endl;
            sc_core::sc_stop();
            return;
        }

        if (!t->get_tlm_transaction()->get_byte_enable_ptr())
        {
            if (t->getMCmd() == gs::Generic_MCMD_RD)
            {
                gs::GSDataType::dtype tmp(&(((uint8_t *)m_ptr)[offset]), size);
                gs::MData mdata(tmp);
                t->setSData(mdata);
            }
            else if (t->getMCmd() == gs::Generic_MCMD_WR)
            {
                /*
                 * FIXME: We should give a bad status for that...
                 */
                if (!m_ro)
                {
                    memcpy(&(((uint8_t *)m_ptr)[offset]), &(t->getMData()[0]),
                           size);
                }
            }
            else
            {
                std::cout << "Invalid command ..." << std::endl;
                sc_core::sc_stop();
                return;
            }
        }
        else
        {
            unsigned int i;
            unsigned char *mask =
                                t->get_tlm_transaction()->get_byte_enable_ptr();
            unsigned int mask_len =
                             t->get_tlm_transaction()->get_byte_enable_length();

            if (size > mask_len)
            {
                std::cout << "Invalid mask length ..." << std::endl;
                sc_core::sc_stop();
                return;
            }

            if (t->getMCmd() == gs::Generic_MCMD_RD)
            {
                for (i = 0; i < size; i++)
                {
                    t->getMData()[i] = (mask[i] == 0xff)
                                       ? ((uint8_t *)m_ptr)[offset + i]
                                       : 0;
                }
            }
            else if (t->getMCmd() == gs::Generic_MCMD_WR)
            {
                if (!m_ro)
                {
                    for (i = 0; i < size; i++)
                    {
                        ((uint8_t *)m_ptr)[offset + i] = (mask[i] == 0xff)
                                               ? t->getMData()[i]
                                               : ((uint8_t *)m_ptr)[offset + i];
                    }
                }
            }
            else
            {
                std::cout << "Invalid command ..." << std::endl;
                sc_core::sc_stop();
                return;
            }
        }

        t->get_tlm_transaction()->set_dmi_allowed(true);
    }

    /*
     * XXX: What about Read Only??
     */
    bool get_direct_mem_ptr(unsigned int from, uint64_t offset,
		            tlm::tlm_generic_payload& payload,
                            tlm::tlm_dmi& dmi_data)
    {
        dmi_data.allow_read();
        dmi_data.set_dmi_ptr((unsigned char *)m_ptr);
        dmi_data.set_start_address(offset);
        dmi_data.set_end_address(offset + m_size * 1024 - 1);
	if (!this->m_ro)
	{
	    dmi_data.allow_write();
	}
        payload.set_dmi_allowed(true);

        return true;
    }

private:
    size_t m_size;
    bool m_ro;
    void *m_ptr;
};

#endif /* BASE_MEMORY_H */
