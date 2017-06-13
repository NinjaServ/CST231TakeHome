
// Seth H. Urbach

module elevator ( clk, reset, FB1, FB2, FB3, CALL1, CALL2, CALL3,
					DOOR, FI, UD  );
	
	input clk, reset, FB1, FB2, FB3, CALL1, CALL2, CALL3;
	output UD;
	output [5:0] DOOR;
	output [7:1] FI;
	reg UD, CD, ClsdDor, resetout;	//CD = Close Door, move = go up a floor
	reg [5:0] DOOR;
	reg [7:1] FI, TFI;
	reg [1:0] state, prevstate, move;
	reg [2:0] count, prevcount, countup;
	reg [3:1] fb, call;

	parameter S0 = 2'b00, S1 = 2'b01, S2 = 2'b10, S3 = 2'b11;
	parameter N0 = 3'b000, N1 = 3'b001, N2 = 3'b010, N3 = 3'b011, N4 = 3'b100, N5 = 3'b101;
	parameter disp1 = 7'b0110000, disp2 = 7'b1101101, disp3 = 7'b1101101;
	parameter close = 6'b111111, open1 = 6'b110011, open2 = 6'b100001, open3 = 6'b000000;

always @ (posedge FB1 or posedge FB2 or posedge FB3 or
			posedge CALL1 or posedge CALL2 or posedge CALL3)
	begin
		fb[1] = FB1;
		fb[2] = FB2;
		fb[3] = FB3;
		call[1] = CALL1;
		call[2] = CALL2;
		call[3] = CALL3;

	end


always @ (posedge clk)
	begin
		//reset, FB1, FB2, FB3, CALL1, CALL2, CALL3;
		prevstate = state;
		prevcount = count;
			case (state)
				S0 :		begin
							count = 0
							countup = 0;
							move = N0;
							state = S1;
							resetout = N1;
							UD = N1;
							TFI = disp1;
						end

				S1 : 	begin
						if (reset && ~CD)
							state = S0;

						else 
							begin
							TFI = disp1;
							case (UD)
								N0 : UD = N1;
								N1 : begin
									UD = N1;

									if (fb[1] || call[1])
										begin
											if (count < 7)
											begin
												count = count + 1;
												state = state;
												CD = 1'b1;
											end
											else
											begin
												count = 0;
												state = state;
												CD = 1'b0;
											end
										end

									else if (fb[2] || call[2] || move == N3 )
									begin
										case (countup)
											N0 :	begin
												if (CD)
												begin
													state = state;
													//move = N0;
													countup = countup;

													if (count < 7)
													begin
														count = count + 1;
														CD = 1'b1;
													end
													else
													begin
														count = 0;
														CD = 1'b0;
													end
												end

												else 
												begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end
												end

											N1 : begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N2 : begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N3 : begin
													state = S2;
													//move = N2;
													countup = 0;
												end
											default : 
												begin
													state = state;
													move = N0;
													countup = 0;
												end
										endcase
									end

									else if (fb[3] || call[3])
									begin
										state = state;
										move = N3;
									end

									else 
									begin
										state = state;
										count = 0;
									end

								default : UD = N1;
								endcase 
							end		 
						end

				S2 : begin
						if (reset && ~CD)
							state = S1;
						else 
							begin
							TF3 = disp2;
							case (UD)
								N0 : begin
									UD = N0;
									
									if (fb[1])		// Checks priority
										UD = N0;
									else if (fb[3] && ClsdDor)
										UD = N1;
									else 
										UD = N0;

									if (fb[2] || call[2])
									begin
											if (count < 7)
											begin
												count = count + 1;
												state = state;
												CD = 1'b1;
											end
											else
											begin
												count = 0;
												state = state;
												CD = 1'b0;
											end
									end

									else if (fb[1] || call[1] || move == N1)
									begin
										case (countup)
											N0 :	begin
												if (CD)
												begin
													state = state;
													//move = N0;
													countup = countup;

													if (count < 7)
													begin
														count = count + 1;
														CD = 1'b1;
													end
													else
													begin
														count = 0;
														CD = 1'b0;
													end
												end

												else 
												begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end
												end

											N1 : begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N2 : begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N3 : begin
													state = S1;
													move = N1;
													countup = 0;
												end
											default : 
												begin
													state = state;
													//move = N0;
													countup = 0;
												end
										endcase
										else 
										begin
											state = state;
											count = 0;
										end
									end

									else if (fb[3] || call[3])
									begin
										case (countup)
											N0 :	begin
												if (CD)
												begin
													state = state;
													//move = N0;
													countup = countup;

													if (count < 7)
													begin
														count = count + 1;
														CD = 1'b1;
													end
													else
													begin
														count = 0;
														CD = 1'b0;
													end
												end

												else 
												begin
													UD = N1;
													state = state;
													//move = N0;
													countup = countup + 1;
												end
												end

											N1 : begin
													UD = N1;
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N2 : begin
													UD = N1;
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N3 : begin
													UD = N1;
													state = S3;
													move = N3;
													countup = 0;
												end
											default : 
												begin
													state = state;
													//move = N0;
													countup = 0;
												end
										endcase

										else 
										begin
											state = state;
											count = 0;
										end
									end
                                             
								N1 : begin
									
									if (fb[3])		// Checks priority
										UD = N1;
									else if (fb[1] && ClsdDor)
										UD = N0;
									else 
										UD = N1;

									if (fb[1] || call[1])
										begin
											if (count < 7)
											begin
												count = count + 1;
												state = state;
												CD = 1'b1;
											end
											else
											begin
												count = 0;
												state = state;
												CD = 1'b0;
											end
										end

									else if (fb[3] || call[3] || move = N3)
									begin
										case (countup)
											N0 :	begin
												if (CD)
												begin
													state = state;
													//move = N0;
													countup = countup;

													if (count < 7)
													begin
														count = count + 1;
														CD = 1'b1;
													end
													else
													begin
														count = 0;
														CD = 1'b0;
													end
												end

												else 
												begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end
												end

											N1 : begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N2 : begin
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N3 : begin
													state = S3;
													move = N3;
													countup = 0;
												end
											default : 
												begin
													state = state;
													move = N0;
													countup = 0;
												end
										endcase
									end
									
									else if (fb[1] || call[1])
									begin
										case (countup)
											N0 :	begin
												if (CD)
												begin
													state = state;
													//move = N0;
													countup = countup;

													if (count < 7)
													begin
														count = count + 1;
														CD = 1'b1;
													end
													else
													begin
														count = 0;
														CD = 1'b0;
													end
												end

												else 
												begin
													UD = N0;
													state = state;
													//move = N0;
													countup = countup + 1;
												end
												end

											N1 : begin
													UD = N0;
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N2 : begin
													UD = N0;
													state = state;
													//move = N0;
													countup = countup + 1;
												end

											N3 : begin
													UD = N0;
													state = S1;
													move = N1;
													countup = 0;
												end
											default : 
												begin
													state = state;
													move = N0;
													countup = 0;
												end
										endcase

										else 
										begin
											state = state;
											count = 0;
										end
									end

								default : UD = N1;
								endcase 
							end		 
					end

				S3 : begin
						if (reset && ~CD)
							state = S2;
						else 
							begin
							TFI = disp3;
							UD = N0;

							if (fb[3] || call[3])
								begin
									if (count < 7)
									begin
										count = count + 1;
										state = state;
										CD = 1'b1;
									end
									else
									begin
										count = 0;
										state = state;
										CD = 1'b0;
									end
								end

							else if (fb[2] || call[2] || move == N2)
								begin
									case (countup)
										N0 :	begin
											if (CD)
											begin
												state = state;
												move = N2;
												countup = countup;

												if (count < 7)
												begin
													count = count + 1;
													CD = 1'b1;
												end
												else
												begin
													count = 0;
													CD = 1'b0;
												end
											end

											else 
											begin
												state = state;
												move = N2;
												countup = countup + 1;
											end
											end

										N1 : begin
												state = state;
												move = N2;
												countup = countup + 1;
											end

										N2 : begin
												state = state;
												move = N2;
												countup = countup + 1;
											end

										N3 : begin
												state = S2;
												move = N2;
												countup = 0;
											end
										default : 
											begin
												state = state;
												move = N2;
												countup = 0;
											end
									endcase

								end
								
								else if (fb[1] || call[1])
								begin
										state = state;
										move = N2;
								end
								
								else 
								begin
										state = state;
										count = 0;
								end

							end		 
					end

				default : begin  state = S1; TFI = disp1;  end
			endcase 
	end


always @ (reset)
	begin
		if (reset && resetout)
			begin
				DOOR = close;	// Default DOOR output is CLOSED
				FI = TFI;		// Output assignment for the Floor Indicator
			end

		else
		begin
		FI = TFI;		// Output assignment for the Floor Indicator

		case (prevstate)
			S1 : begin
					if (prevcount == 0 || prevcount == 7)
						DOOR = close;
					else if (prevcount == 1 || prevcount == 6)
						DOOR = open1;
					else if (prevcount == 2 || prevcount == 5)
						DOOR = open2;
					else if (prevcount == 3 || prevcount == 4)
						DOOR = open3;
					else
						DOOR = close;
				end

			S2 : begin
					if (prevcount == 0 ||prevcount == 7)
						DOOR = close;
					else if (prevcount == 1 || prevcount == 6)
						DOOR = open1;
					else if (prevcount == 2 || prevcount == 5)
						DOOR = open2;
					else if (prevcount == 3 || prevcount == 4)
						DOOR = open3;
					else
						DOOR = close;
				end

			S3 : begin
					if (prevcount == 0 || prevcount == 7)
						DOOR = close;
					else if (prevcount == 1 || prevcount == 6)
						DOOR = open1;
					else if (prevcount == 2 || prevcount == 5)
						DOOR = open2;
					else if (prevcount == 3 || prevcount == 4)
						DOOR = open3;
					else
						DOOR = close;
				end

			default : DOOR = close;
		endcase 
			end
	end



endmodule


//-----------------------------------------------------------------------------------//



// o1 : DOOR = 6'b111111;
//		FI = 7'b0110000;
// o1 : DOOR = 6'b110011;
//		FI = 7'b0110000;
// o1 : DOOR = 6'b100001;
//		FI = 7'b0110000;
// o1 : DOOR = 6'b000000;
//		FI = 7'b0110000;

// o2 : DOOR = 6'b111111;
//		FI = 7'b1101101;
// o2 : DOOR = 6'b110011;
//		FI = 7'b1101101;
// o2 : DOOR = 6'b100001;
//		FI = 7'b1101101;
// o2 : DOOR = 6'b000000;
//		FI = 7'b1101101;

// o3 : DOOR = 6'b111111;
//		FI = 7'b1101101;
// o3 : DOOR = 6'b110011;
//		FI = 7'b1101101;
// o3 : DOOR = 6'b100001;
//		FI = 7'b1101101;
// o3 : DOOR = 6'b000000;
//		FI = 7'b1101101;

//DOOR = {FB1, FB2, FB3, CALL1, CALL2, CALL3};
		//FI = {reset, FB1, FB2, FB3, CALL1, CALL2, CALL3};
		//UD = reset;



		begin
						TFI = disp1;
						case (UD)
							N0 : UD = N1;
							N1 : 	begin
										UD = N1;
										if (FB1 or CALL1)
											begin
												TFI = 7'b0110000;
											end
										else if (FB2 or CALL2)
											begin
												case (count)
													N0 :	begin
															if (CD)
															begin
															DOOR = close;
															TFI = disp1;
															state = state;
															end
															else 
															begin
															DOOR = close;
															TFI = disp1;
															state = state;
															move = N1;
															end
														end
													N1 : 
													N2 :
													N3 :
													N4 :

												if (count != 4 and )		
												count = count + 1;
											end
													
													

											end
											

									end
							default : UD = N1;
									 
						end

				S2 : FI = disp2;
				S3 : FI = disp3;

				default : begin state = S1; FI = disp1; end

	end












	module elevator ( clk, reset, FB1, FB2, FB3, CALL1, CALL2, CALL3,
					DOOR, FI, UD  );
	
	input clk, reset, FB1, FB2, FB3, CALL1, CALL2, CALL3;
	output UD;
	output [5:0] DOOR;
	output [7:1] FI;
	reg UD;
	reg [5:0] DOOR;
	reg [7:1] FI;
	reg [1:0] state, prevstate, setstate, storestate;

parameter S1 = 3'b000, S2 = 3'b001, S3 = 3'b010, S4 = 3'b011;
parameter N0 = 3'b000, N1 = 3'b001, N2 = 3'b010, N3 = 3'b011, N4 = 3'b100, N5 = 3'b101;


always @ (posedge clk)
	begin
			case (state)
				S1 : 	begin
						case (UD)
							N0 : UD = N1;
							N1 : 	begin
										UD = N1;
										if (FB1 or CALL1)
											begin

											end


									end
									
						end

				S2 :
				S3 :	
				default : state = S1;


//DOOR = {FB1, FB2, FB3, CALL1, CALL2, CALL3};
		//FI = {reset, FB1, FB2, FB3, CALL1, CALL2, CALL3};
		//UD = reset;
	end


endmodule


if (count == 0)
											begin
												TFI = disp1;
												state = S2;
												//move = N0;
												count = count;
											end

										else if (count == 1 || count == 6)
											begin
												TFI = disp1;
												state = S2;
												move = N1;
												count = count + 1;
											end
											
										else if (count == 0 || count == 7)
											begin
												TFI = disp1;
												state = S2;
												move = N1;
												count = 0;
											end
											
										else if (count == 0 || count == 7)
											begin
												TFI = disp1;
												state = S2;
												move = N1;
												count = 0;
											end
											

												N1 : begin
														TFI = disp1;
														state = S2;
														move = N1;
													end

												N2 : begin
														TFI = disp1;
														state = S2;
														move = N1;
													end

												N3 : begin
														TFI = disp1;
														state = S2;
														move = N1;
													end

												N4 : begin
														TFI = disp1;
														state = S2;
														move = N1;
													end
											endcase

											if (count < 7)		
												count = count + 1;
											else
												count = 0;
										end




if read and prev fb1
then ----
default
door = closed,
dipsplay = displayfloor;;
