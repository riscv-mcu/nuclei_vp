/*
* @file DualPortMemory.h
* @author Guillaume Delbergue <guillaume.delbergue@greensocs.com>
* @date May, 2015
* @copyright Copyright (C) 2015, GreenSocs Ltd.
*
* @brief Dual port memory for SystemC using GreenLib.
*/

#ifndef DUAL_PORT_MEMORY_H
#define DUAL_PORT_MEMORY_H

#include <systemc>
#include <gsgpsocket/transport/GSGPSlaveSocket.h>

#define BASE_MEMORY_INCLUSION_PROTECTOR 
#include "SimpleMemory/BaseMemory.h"
#undef BASE_MEMORY_INCLUSION_PROTECTOR

template <unsigned int BUSWIDTH_0, unsigned int BUSWIDTH_1>
class DualPortMemory:
  public sc_core::sc_module,
  public BaseMemory
{
public:
    DualPortMemory(sc_core::sc_module_name name):
    sc_core::sc_module(name),
    BaseMemory(),
    targetPort0("target_port_0"),
    targetPort1("target_port_1"),
    m_size("size", 1000),
    m_ro("read_only", false)    
    {
        wrap_1.register_b_transport(&DualPortMemory::b_transact_0, this);
        this->targetPort0.bind_b_if(wrap_1);
	this->targetPort0.register_transport_dbg(this,
					 &DualPortMemory::dbg_transact_0);
        this->targetPort0.register_get_direct_mem_ptr(this,
            &DualPortMemory::get_direct_mem_ptr_0);

        wrap_2.register_b_transport(&DualPortMemory::b_transact_1, this);
        this->targetPort1.bind_b_if(wrap_2);
	this->targetPort1.register_transport_dbg(this,
					 &DualPortMemory::dbg_transact_1);
        this->targetPort1.register_get_direct_mem_ptr(this,
            &DualPortMemory::get_direct_mem_ptr_1);
    }

    unsigned int dbg_transact_0(unsigned int index,
                                tlm::tlm_generic_payload& payload)
    {
        uint64_t offset = payload.get_address() - targetPort0.base_addr;
        return BaseMemory::dbg_transport(payload, offset);
    }

    unsigned int dbg_transact_1(unsigned int index,
                                tlm::tlm_generic_payload& payload)
    {
        uint64_t offset = payload.get_address() - targetPort1.base_addr;
        return BaseMemory::dbg_transport(payload, offset);
    }

    void b_transact_0(gs::gp::GenericSlaveAccessHandle ah)
    {
        uint32_t offset;
        accessHandle0 t = _getSlaveAccessHandle(ah);

        offset = t->getMAddr() - targetPort0.base_addr;
        BaseMemory::b_transact(t, offset);
    }

    void b_transact_1(gs::gp::GenericSlaveAccessHandle ah)
    {
        uint32_t offset;
        accessHandle1 t = _getSlaveAccessHandle(ah);

        offset = t->getMAddr() - targetPort1.base_addr;
        BaseMemory::b_transact(t, offset);
    }

    bool get_direct_mem_ptr_0(unsigned int from,
			      tlm::tlm_generic_payload& payload,
			      tlm::tlm_dmi& dmi_data)
    {
        /* Wrap get_direct_mem_ptr() to get the port offset in BaseMemory */

        return BaseMemory::get_direct_mem_ptr(from, targetPort0.base_addr,
                                              payload, dmi_data);
    }

    bool get_direct_mem_ptr_1(unsigned int from,
			      tlm::tlm_generic_payload& payload,
			      tlm::tlm_dmi& dmi_data)
    {
        /* Wrap get_direct_mem_ptr() to get the port offset in BaseMemory */

        return BaseMemory::get_direct_mem_ptr(from, targetPort1.base_addr,
                                              payload, dmi_data);
    }


    void end_of_elaboration()
    {
      this->allocate_memory(m_size, m_ro);
    }

    // Sockets for memory access
    typedef typename gs::gp::GenericSlavePort<BUSWIDTH_0>::accessHandle
                                                               accessHandle0;
    gs::gp::GenericSlavePort<BUSWIDTH_0> targetPort0;
    typedef typename gs::gp::GenericSlavePort<BUSWIDTH_1>::accessHandle
                                                               accessHandle1;
    gs::gp::GenericSlavePort<BUSWIDTH_1> targetPort1;

  private:
    gs::gs_param<uint32_t> m_size;
    gs::gs_param<bool> m_ro; /*!< read only? */
    gs::tlm_b_if_wrapper<DualPortMemory,
                         gs::gp::GenericSlaveAccessHandle> wrap_1;
    gs::tlm_b_if_wrapper<DualPortMemory,
                         gs::gp::GenericSlaveAccessHandle> wrap_2;
};

#endif /* DUAL_PORT_MEMORY_H */
