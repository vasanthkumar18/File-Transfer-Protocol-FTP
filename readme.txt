STEP 1: make a txt file with a name "capture.txt"

STEP 2: Enter the command : ifstat -l -t -i lo 0.1>capture.txt 	===== 	TRANSIMISSION RATES 

STEP 3: Run the program

STEP 4: Stop the capturing 

STEP 5: open gnuplot

	Terminal type is now 'qt'
	gnuplot> set xlabel "Time"
	gnuplot> set ylabel "Transmission rate - KB/s"
	gnuplot> plot "capture.txt" 2:xtic[10] with boxes
	
	The plot will be displayed. 
	
Name: BHUKYA VASANTH KUMAR
B180441CS

