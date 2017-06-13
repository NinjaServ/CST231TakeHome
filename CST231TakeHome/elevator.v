


module elevator ( clk, reset, FB1, FB2, FB3, CALL1, CALL2, CALL3,
					DOOR, FI, UD, count  );
	
	input clk, reset, FB1, FB2, FB3, CALL1, CALL2, CALL3;
	output UD;
	output [5:0] DOOR;
	output [7:1] FI;
	output [2:0] count;	

	reg UD, CD, ClsdDor, resetout, read;	//CD = Close Door, move = go up a floor
	reg [5:0] DOOR;
	reg [7:1] FI, nextFI, prevFI;
	reg [2:0] state, prevstate, nextstate ;
	reg [2:0] count, prevcount, countup;
	reg [3:1] fb, call;


	parameter S0 = 3'b000, S1 = 3'b001, S2 = 3'b010, S3 = 3'b011, 
				S4 = 3'b100, S5 = 3'b101, S6 = 3'b110, S7 = 3'b111 ;
	parameter N0 = 3'b000, N1 = 3'b001, N2 = 3'b010, N3 = 3'b011, N4 = 3'b100, N5 = 3'b101;
	parameter disp1 = 7'b0110000, disp2 = 7'b1101101, disp3 = 7'b1101101;
	parameter close = 6'b111111, open1 = 6'b110011, open2 = 6'b100001, open3 = 6'b000000;

always @ (FB1 or FB2 or  FB3 or CALL1 or CALL2 or CALL3)
	begin
		if (~FB1 && read)
			fb[1] = ~FB1 ;
		else
			fb[1] = fb[1] ;

		if (~FB2 && read)
			fb[2] = ~FB2;
		else
			fb[2] = fb[2] ;

		if (~FB3 && read)
			fb[3] = ~FB3;
		else
			fb[3] = fb[3] ;


		if (~CALL1 && read)
			call[1] = ~CALL1;
		else
			call[1] = call[1];

		if (~CALL2 && read)
			call[2] = ~CALL2;
		else
			call[2] = call[2];

		if (~CALL3 && read)
			call[3] = ~CALL3;
		else
			call[3] = call[3];

	end


always @ (posedge clk)
	begin

		prevstate = state;
		prevcount = count;

		if (reset)
			begin
				state = S0;
				UD = N1;
				FI =disp1;
				count = N0;
			end



		case (state)
				S0 :	//init
						begin
							read = N1;
							if ( fb[1] || fb[1] || fb[2] || fb[3] ||
									call[1] ||	call[2] || call[3] )
							begin
								state = S4;
								nextstate = S1;
								UD = 1'b1;
								count = 0;
								FI = disp1;
							end
		
							else
							begin
								state = state;
								UD = 1'b1;
								count = 0;
								FI = disp1;
							end
						end


				S1 :	//idle
						begin
							read = N1;
						
							UD = N1;
							FI =disp1;
							count = 0;
							state = S2;
							nextstate = S1;
							
							prevFI = disp1;
							nextFI = disp2;
							
						end

				S2:		//open
						begin
							if (count < 3)
								begin
									count = count + 1'b1;
									state = state;
								end

							else
								begin
									count = 0;
									state = nextstate ;
									CD = 1'b1;
								end
						end


				S3:		//close
						begin
							if (count < 3)
								begin
									count = count + 1'b1;
									state = state;
								end

								else
									begin
										count = 0;
										state = nextstate ;
										CD = 1'b0;
									end


						end


						
				S4:		//up
						begin
						if (count < 3)
						begin
							count = count + 1'b1;
							UD = 1'b1;
							state = S4;
							FI = prevFI;
						end
					
						else
						begin
							count = 0;
							UD = 1'b1;
							state = nextstate;
							FI = nextFI;
						end
						end

				S5:		//down
					begin
					if (count < 3)
						begin
							count = count + 1'b1;
							UD = 1'b0;
							state = S4;
							FI = prevFI;
						end
					
						else
						begin
							count = 0;
							UD = 1'b0;
							state = nextstate;
							FI = nextFI;
						end
					end
		endcase

						

		case (prevcount)
			N0 : DOOR = close;
			N1 : DOOR = open1;
			N2 : DOOR = open2;
			N3 : DOOR = open3;
			default :	DOOR = close;
		endcase		
			
	end


endmodule

