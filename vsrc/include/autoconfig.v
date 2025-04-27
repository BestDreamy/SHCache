
parameter int numRNs = 4;
parameter int numHNs = 1;
parameter int numSNs = 1;

parameter int numCreditsForHNReq [0:0] = '{4};
parameter int numCreditsForSNReq [0:0] = '{4};

parameter int RNId [0:3] = '{'hc, 'h2c, 'h3c, 'h4c};
parameter int HNId [0:0] = '{'h28};
parameter int SNId [0:0] = '{'h24};
