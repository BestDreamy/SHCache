`include "chi_intf"
`include "autoconfig"

module switch(
    input clock,
    input reset,
    // chi_hn_rxreq_intf rxreq,
    output f
);

assign f = clock;

endmodule