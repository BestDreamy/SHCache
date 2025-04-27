`ifndef CHI_INTF_H
`define CHI_INTF_H

`include "chi_flit"

interface chi_hn_rxreq_intf;
    input reqflit_t     flit;
    input logic         flitv;
    input logic         flitpend;

    output logic        lcrdv;

endinterface

`endif