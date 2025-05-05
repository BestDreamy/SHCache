`ifndef CHI_INTF_H
`define CHI_INTF_H

`include "chi_flit.vh"

interface chi_hn_req_intf;
    reqflit_t     flit;
    logic         flitv;
    logic         flitpend;

    logic         lcrdv;

    modport RX (
        input  flit,
        input  flitv,
        input  flitpend,
        output lcrdv
    );

    modport TX ( // Transmitter
        output flit,
        output flitv,
        output flitpend,
        input  lcrdv
    );

endinterface

`endif