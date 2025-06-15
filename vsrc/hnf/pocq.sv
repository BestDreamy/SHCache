module pocq #(
    parameter DEPTH = 16
)(
    input                   req_entry_en,
    input reqflit_t         req_entry,
    input                   rsp_entry_en,
    input rspflit_t         rsp_entry,
    input                   read_no_snp_v,
    input reqflit_t         read_no_snp,

    output logic            pocq_is_empty, // Check whether transaction is finished

    input                   clock,
    input                   reset
);
    reqflit_t                           buffer [DEPTH-1:0];
    reg                                 [DEPTH-1:0]  valid;
    // reg                                 sleep  [DEPTH-1:0];

    assign pocq_is_empty = (valid == '0); // pocq is empty when all valid bits are 0

    // To push the req_entry into this pocq, find first empty entry
    reg [$clog2(DEPTH)-1:0] pocq_first_valid_entry;

    always @(*) begin : find_pocq_first_valid_entry
        pocq_first_valid_entry = '0;
        for (int i = 0; i < DEPTH; i = i + 1) begin
            if (~valid[i] && pocq_first_valid_entry == '0) begin
                pocq_first_valid_entry = i[$clog2(DEPTH)-1:0];
            end
        end
    end

    // To replace the req_entry, find the valid entry according to readNoSnp
    // When read_no_snp is valid, it means the pocq has a req_entry that needs to be replaced
    reg [$clog2(DEPTH)-1:0] pocq_replace_entry;

    always @(*) begin : find_pocq_replace_entry
        pocq_replace_entry = '0;
        for (int i = 0; i < DEPTH; i = i + 1) begin
            if (valid[i] && pocq_replace_entry == '0 && read_no_snp_v && 
                buffer[i].TxnID == read_no_snp.ReturnTxnID &&
                buffer[i].SrcID == read_no_snp.StashNID_ReturnNID) begin
                pocq_replace_entry = i[$clog2(DEPTH)-1:0];
            end
        end
    end

    // To release the req_entry, find the valid entry according to response
    // When CompAck is valid, it means the pocq has a req_entry that needs to be released
    reg [$clog2(DEPTH)-1:0] pocq_release_entry;

    always @(*) begin : find_pocq_release_entry
        pocq_release_entry = '0;
        for (int i = 0; i < DEPTH; i = i + 1) begin
            if (valid[i] && pocq_release_entry == '0 && rsp_entry_en && 
                buffer[i].TxnID == rsp_entry.TxnID &&
                buffer[i].SrcID == rsp_entry.TgtID) begin
                pocq_release_entry = i[$clog2(DEPTH)-1:0];
            end
        end
    end

    always @(posedge clock) begin: req_entry_into_buffer
        if (reset) begin
            for (int i = 0; i < DEPTH; i = i + 1) begin
                buffer[i] <= 'b0;
                valid[i]  <= 'b0;
                // sleep[i]  <= 'b0;
            end
        end else if (req_entry_en) begin
            buffer[pocq_first_valid_entry] <= req_entry;
            valid[pocq_first_valid_entry] <= 1'b1;
            // sleep[pocq_first_valid_entry] <= 1'b1; // fill buffer && sleep
        end else if (read_no_snp_v) begin
            buffer[pocq_replace_entry] <= read_no_snp;
            valid[pocq_replace_entry] <= 1'b1;
        end else if (rsp_entry_en) begin
            buffer[pocq_release_entry] <= 'b0; // release the entry
            valid[pocq_release_entry] <= 1'b0;
        end
    end

    always_comb begin
        assert((pocq_release_entry != pocq_replace_entry) && read_no_snp_v && rsp_entry_en) else
            $error("Replace a req entry and release a req entry at the same time, which is not allowed in pocq");
    end
endmodule