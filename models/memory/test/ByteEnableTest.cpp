/*
 * ByteEnableTest.cpp
 *
 * Copyright (C) 2016, GreenSocs Ltd.
 *
 * Developed by Konrad Frederic <fred.konrad@greensocs.com>
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

#include "SimpleMemory/simpleMemory.h"
#include <greencontrol/config.h>
#include <gsgpsocket/transport/GSGPMasterBlockingSocket.h>
#include <gsgpsocket/transport/GSGPSlaveSocket.h>
#include <gsgpsocket/transport/GSGPconfig.h>

#define TEST(str) std::cout << "testing: " << str << std::endl;
#define ERROR(str) std::cout << "failed: " << str << std::endl;
#define SUCCESS() std::cout << "success" << std::endl;
#define INFO(str) std::cout << "info: " << str << std::endl;

class TestMaster:
  public sc_module,
  public gs::payload_event_queue_output_if<gs::gp::master_atom>
{
    public:
    SC_HAS_PROCESS(TestMaster);
    TestMaster(sc_module_name name):
    sc_module(name),
    master_socket("master_port")
    {
        master_socket.out_port(*this);
        fail = false;
        SC_THREAD(testing);
    };

    ~TestMaster() {};

    void notify(gs::gp::master_atom& tc) {};

    void end_of_elaboration()
    {
        tHandle = master_socket.create_transaction();
    }

    typedef gs::gp::GenericMasterBlockingPort<32>::accessHandle
                                                            transactionHandle;
    gs::gp::GenericMasterBlockingPort<32> master_socket;

    bool fail;

    private:
    transactionHandle tHandle;

    bool testResponse()
    {
        if (tHandle->getSResp() != gs::Generic_SRESP_DVA)
        {
            return false;
        }
        return true;
    }

    void write(uint64_t address, const uint32_t value,
               unsigned char *byte_enable, unsigned int byte_enable_len)
    {
        gs::GSDataType::dtype data =
            gs::GSDataType::dtype((unsigned char *)&value, sizeof(value));
        tHandle->setMBurstLength(4);
        tHandle->setMAddr(address);
        tHandle->setMData(data);
        tHandle->setMCmd(gs::Generic_MCMD_WR);
        tHandle->get_tlm_transaction()->set_byte_enable_ptr(byte_enable);
        tHandle->get_tlm_transaction()->set_byte_enable_length(byte_enable_len);
        master_socket.Transact(tHandle);

        if (!testResponse())
        {
            this->fail = true;
        }
    }

    uint32_t read(uint64_t address, unsigned char *byte_enable,
                  unsigned int byte_enable_len)
    {
        uint32_t value = 0;
        gs::GSDataType::dtype data =
            gs::GSDataType::dtype((unsigned char *)&value, sizeof(value));
        tHandle->setMBurstLength(4);
        tHandle->setMAddr(address);
        tHandle->setMData(data);
        tHandle->setMCmd(gs::Generic_MCMD_RD);
        tHandle->get_tlm_transaction()->set_byte_enable_ptr(byte_enable);
        tHandle->get_tlm_transaction()->set_byte_enable_length(byte_enable_len);
        master_socket.Transact(tHandle);
        value = *((uint32_t *)data.getData());

        if (!testResponse())
        {
            this->fail = true;
        }
        return value;
    }

    void testing()
    {
        uint32_t w_mask = 0xFF00FF00;
        uint32_t r_mask = 0x0000FF00;
        uint32_t value_w = 0xDEADBEEF;
        uint32_t value_r = 0;

        TEST("Writing 0xDEADBEEF with a 0xFF00FF00 masks.");
        this->write(0, value_w, (unsigned char *)&w_mask, sizeof(w_mask));
        if (this->fail)
        {
            ERROR("Failed to write to the memory.");
            return;
        }

        value_r = this->read(0, NULL, 0);
        if (this->fail)
        {
            ERROR("Failed to read from the memory.");
            return;
        }
        if (value_r != (value_w & w_mask))
        {
            ERROR("The write masking failed: expected " << (value_w & w_mask)
                  << " got " << value_r);
            this->fail = true;
            return;
        }

        value_r = this->read(0, (unsigned char *)&r_mask, sizeof(r_mask));
        if (this->fail)
        {
            ERROR("Failed to read from the memory.");
            this->fail = true;
            return;
        }
        if (value_r != (value_w & w_mask & r_mask))
        {
            ERROR("The read masking failed: expected "
                  << (value_w & w_mask & r_mask) << " got " << value_r);
            this->fail = true;
            return;
        }

        SUCCESS();
    };
};

int sc_main(int argc, char *argv[])
{
    Memory<32> *mem = new Memory<32>("mem");
    TestMaster *initiator = new TestMaster("initiator");

    initiator->master_socket(mem->target_port);
    sc_start();

    return initiator->fail;
}
