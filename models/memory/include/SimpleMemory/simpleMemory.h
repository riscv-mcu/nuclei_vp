/*
 * simpleMemory.h
 *
 * Copyright (C) 2014, GreenSocs Ltd.
 *
 * Developped by Konrad Frederic <fred.konrad@greensocs.com>
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
 * This is a simple Memory for SystemC using GreenLib.
 */

#ifndef MEMORY_H
#define MEMORY_H

//#define MEMORY_DEBUG
#ifdef MEMORY_DEBUG
#define DCOUT(var) std::cout << "Memory: " << sc_core::sc_time_stamp() << " :" \
                     << var << std::endl;
#else
#define DCOUT(var)
#endif

#include <systemc.h>
#include <gsgpsocket/transport/GSGPSlaveSocket.h>

#define BASE_MEMORY_INCLUSION_PROTECTOR 
#include "SimpleMemory/BaseMemory.h"
#undef BASE_MEMORY_INCLUSION_PROTECTOR

template <unsigned int BUSWIDTH>
class Memory:
  public sc_module,
  public BaseMemory,
  public gs::tlm_b_if<gs::gp::GenericSlaveAccessHandle>
{
  public:
    Memory(sc_core::sc_module_name name):
    sc_module(name),
    BaseMemory(),
    target_port("target_port"),
    m_size("size", 262144),
    m_ro("read_only", false)
    {
      this->target_port.bind_b_if(*this);
      this->target_port.register_get_direct_mem_ptr(this,
                                                   &Memory::get_direct_mem_ptr);
      this->target_port.register_transport_dbg(this, &Memory::dbg_transact);
    }

    ~Memory()
    {
    }

    void end_of_elaboration()
    {
      this->allocate_memory(m_size, m_ro);
    }

    void b_transact(gs::gp::GenericSlaveAccessHandle ah)
    {
      accessHandle t = _getSlaveAccessHandle(ah);
      uint32_t offset = t->getMAddr() - target_port.base_addr;

      BaseMemory::b_transact(t, offset);
    }

    unsigned int dbg_transact(unsigned int index,
                              tlm::tlm_generic_payload& payload)
    {
        uint64_t offset = payload.get_address() - target_port.base_addr;
        return BaseMemory::dbg_transport(payload, offset);
    }

    bool get_direct_mem_ptr(unsigned int from,
			    tlm::tlm_generic_payload& payload,
			    tlm::tlm_dmi& dmi_data)
    {
        /* Wrap get_direct_mem_ptr() to get the port offset in BaseMemory */

        return BaseMemory::get_direct_mem_ptr(from, target_port.base_addr,
					      payload, dmi_data);
    }

    /*
     * Socket for memory access.
     */
    typename gs::gp::GenericSlavePort<BUSWIDTH> target_port;
    typedef typename gs::gp::GenericSlavePort<BUSWIDTH>::accessHandle
                                                                  accessHandle;
  private:
    gs::gs_param<uint32_t> m_size;
    gs::gs_param<bool> m_ro; /*!< read only? */
};

#endif /* MEMORY_H */
