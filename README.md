# Real-time-Clock-for-ATSAMD51
Arduino example demonstrating use of ATSAMD51 RTC peripheral

I implemented a filter (not included here, but you can find details at https://www.eecis.udel.edu/~mills/database/brief/arch/arch.pdf) on NTP updates of my clock. An NTP update is requested every 10 minutes and the offset and transit delay (T4 - T1) - (T3 - T2) are recorded in an array. After accumulating 8 updates (I'm getting them from pool.ntp.org, and if a server is returned that isn't available I skip that update), I find the one with the shortest delay and use it to update the clock if the offset is non-zero. I then clear the array and start over.

I've had this running on a Metro M4 Express since May 4. On August 16 I imported the log file into Excel and some simple analysis revealed:

Avg update value (seconds): 0.0000000
StDev: 0.142531528 
Number of non-zero updates: 52
Sum of updates: 0
Total number of updates recorded: 2,856

Note that I'm only using the integer portions of the times returned by NTP servers, and suspect this accounts for the occasional jitter where I'll see an update of +1 followed by another of -1 in the log.
