
`ifndef CONFIG_H
`define CONFIG_H

parameter int numRNs = 4;
parameter int numHNs = 1;
parameter int numSNs = 1;

parameter int numCreditsForHNReq [0:0] = '{4};
parameter int numCreditsForSNReq [0:0] = '{4};

parameter int RNId [0:3] = '{'h0, 'h1, 'h2, 'h3};
parameter int HNId [0:0] = '{'h4};
parameter int SNId [0:0] = '{'h7};

parameter int RNsOffset [0:0] = '{4};
`endif
