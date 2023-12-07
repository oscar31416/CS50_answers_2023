-- Keep a log of any SQL queries you execute as you solve the mystery.
/* GOALS:
   > Name of thief?
   > Thief took a flight shortly afterwards. Destination?
   > Accomplice? If any, name(s)?

   STARTING POINT:
   > Place of theft: Humphrey Street.
   > Date of theft: 2021-07-28. */


/*************/
/* ATTEMPT 1 */
/*************/

/* TASK 1 - Read reports from the seventh month of 2021. */
SELECT QUOTE('TASK 1');
SELECT QUOTE('REPORTS FROM JULY 2021');
SELECT year,month,day,id,description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28;

/* RESULTS:
   > Duck stolen at 10:15 at the Humphrey Street bakery.
   > Three witnesses interviewed on the same day.
   > Transcriptions available. They all mention the bakery.

   DEDUCTED FROM DATA:
   > Flight must have taken place AFTER 10:15. */

/* TASK 2 - Find interviews starting from 2021-07-28
   to the end of July. */
SELECT QUOTE('');
SELECT QUOTE('TASK 2 - FETCH INTERVIEWS ON 2021-07-28');
SELECT year,month,day,id FROM interviews
WHERE year = 2021 AND month = 7 AND day >= 28;

/* RESULTS:
   > Expected 3 interviews. 7 found.

   QUESTIONS:
   > May I need transcripts from interviews after 2021-07-28? */

/* TASK 3 - Read scripts of interviews that took place on 2021-07-28
   and find those that mention the bakery. */
SELECT QUOTE('');
SELECT QUOTE('TASK 3 - READ TRANSCRIPTS FROM INTERVIEWS ON 2021-07-28');
SELECT year,month,day,id,transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;

/* RESULTS:
   > Interview on 2021-07-28, ID 161
   --> Thief got on a car by, or before, 10:25.
   > Interview on 2021-07-28, ID 162
   --> Thief looks familiar to interviewee.
   --> BEFORE theft, money withdrawn from ATM on Leggett Street.
   > Interview on 2021-07-28, ID 163
   --> After theft, phone call. Less than a minute.
   --> Thief and the other person plan to leave Fiftyville by plane quickly.
   --> That other person is asked to buy flight tickets.

   DEDUCTED FROM DATA:
   > Possible accomplice. Several???

   FIXED DEDUCTIONS:
   > Flight must have taken place after ~10:25.

   QUESTIONS:
   --> Traffic cameras near the bakery? If so...
   ----> Look for number plates between 10:15 and 10:25 approximately.
   ----> Look for owner of car with number plate in question.
         UNRELIABLE. The car might have been stolen.
   --> Money taken from ATM on Leggett Street? For what?
   ----> Fetch data from transactions on 2021-07-28 AND before theft.
   --> Phone call after theft.
   ----> Numbers involved?
   ------> Owners?
   ----> Check start time and duration. */

/* TASK 4 - Check bakery's traffic log on 2021-07-28 AND until 11:00. */
SELECT QUOTE('');
SELECT QUOTE('TASK 4 - SELECT CARS THAT LEFT THE CAR PARK ON 2021-07-28 UNTIL 11:00');
SELECT id,hour,minute,activity,license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour >= 0 AND hour < 11;

/* RESULTS:
   > Found 8 number plates of vehicles that exit between 10:15 and 10:25:
   --> 5P2BI95 enters at 09:15 and leaves at 10:16
   --> 94KL13X enters at 08:23 and leaves at 10:18
   --> 6P58WS2 enters at 09:20 and leaves at 10:18
   --> 4328GD8 enters at 09:14 and leaves at 10:19
   --> G412CB7 enters at 09:28 and leaves at 10:20
   --> L93JTIZ enters at 08:18 and leaves at 10:21
   --> 322W7JE enters at 08:36 and leaves at 10:23
   --> 0NTHK55 enters at 08:42 and leaves at 10:23

   TOO MANY NUMBER PLATES.
   Phone calls might help. */

/* TASK 5 - Check phone calls on 2021-07-28. Select those that last less than a minute. */
SELECT QUOTE('');
SELECT QUOTE('TASK 5 - SELECT PHONE CALLS ON 2021-07-28 THAT LASTED LESS THAN A MINUTE');
SELECT caller,receiver,duration FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60;

/* RESULTS:
   > 10 calls found.
   --> (130) 555-0289|(996) 555-8899|51
   --> (499) 555-9472|(892) 555-8872|36
   --> (367) 555-5533|(375) 555-8161|45
   --> (609) 555-5876|(389) 555-5198|60
   --> (499) 555-9472|(717) 555-1342|50
   --> (286) 555-6063|(676) 555-6554|43
   --> (770) 555-1861|(725) 555-3243|49
   --> (031) 555-6622|(910) 555-3251|38
   --> (826) 555-1652|(066) 555-9701|55
   --> (338) 555-6650|(704) 555-2131|54

   TOO MANY PHONE CALLS, and no information about time of beginning.

   POSSIBLE SOLUTION TO THIS PROBLEM:
   > For each of the 8 number plates, find owner and phone number.
   > Then, check phone calls. Accomplice might be the receiver.*/

/* TASK 6 - Check if any of the exiting cars' owners received
   a call on 2021-07-28 with a duration of a minute or less. */
SELECT QUOTE('');
SELECT QUOTE('TASK 6 - CROSS DATA FROM TASKS 4 AND 5 VIA');
SELECT QUOTE('         OWNER OF PHONE AND CAR');
SELECT caller,receiver,duration FROM phone_calls
WHERE receiver IN (SELECT phone_number FROM people
WHERE license_plate in (SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND minute <= 30 AND activity = 'exit'))
AND year = 2021 AND month = 7 AND day = 28 AND duration <= 60;

/* RESULTS:
   > ONE PHONE CALL:
   --> From: (609) 555-5876
   --> To:   (389) 555-5198
   --> Duration (s):     60

   > BEST-CASE SCENARIO:
   --> Caller is the thief.
   ----> ID = 561160
   ----> NAME = Kathryn
   ----> PHONE NUMBER = (609) 555-5876
   ----> PASSPORT NUMBER = 6121106406
   ----> NUMBER PLATE = 4ZY7I8T
   --> Receiver is owner of car and accomplice.
   ----> ID = 467400
   ----> NAME = Luca
   ----> PHONE NUMBER = (389) 555-5198
   ----> PASSPORT NUMBER = 8496433585
   ----> NUMBER PLATE = 4328GD8

   > WORST-CASE SCENARIO:
   --> Stolen car.
   --> Phones acquired (bought or stolen) just for theft
       and disposed of afterwards.

   > DEDUCTED FROM DATA:
   --> Luca's car leaves the car park between 10:15 and 10:25.

   > WHAT WE'VE GOT AT THE MOMENT:
   --> Suspects: Kathryn (thief), Luca (accomplice).

   > WHAT NEEDS TO BE DISCOVERED AND HOW?
   --> Details of Kathryn's flights on 2021-07-28. */

/* TASK 7 - Get IDs of all of Kathryn's flights.
   Each password number can be linked to all the flights
   that a person took.*/
SELECT QUOTE('');
SELECT QUOTE('TASK 7 - GET IDENTIFIERS OF KATHRYN\'S FLIGHTS')
SELECT flight_id FROM passengers WHERE passport_number = 6121106406;

/* RESULTS:
   > One flight ID: 34

   DEDUCTED FROM DATA:
   > Kathryn has only flown once.
   --> We need the details of that flight!! */

/* TASK 8 - Fetch the information about flight 34.
   I'll specify the desired date and time interval. */
SELECT QUOTE('');
SELECT QUOTE('TASK 8 - FETCH INFORMATION ABOUT KATHRYN\'S ONLY FLIGHT');
SELECT id,origin_airport_id,destination_airport_id,year,month,day,hour,minute FROM flights
WHERE id = 34 AND year = 2021 AND month = 7 AND day = 28 AND hour >= 10;

/* RESULTS:
   > Origin airport ID: 8
   > Destination airport ID: 5
   > Date: 2021-07-28
   > Departure time: 17:20 */

/* TASK 9 - Fetch the origin and the destination of the flight. */
SELECT QUOTE('');
SELECT QUOTE('TASK 9 - FETCH ORIGIN AND DESTINATION CITIES');
SELECT city FROM airports WHERE id = 8;
SELECT city FROM airports WHERE id = 5;

/* RESULTS:
   > Flight departed from FIFTYVILLE.
   > Flight arrived in DALLAS.

   RECREATED PROCESS:
   > Luca enters the car park at 09:14.
   > Kathryn steals the CS50 duck at 10:15.
     Three people witness the event.
   > After the theft, Kathryn calls Luca,
     owner of the getaway car.
   > Luca's car leaves the car park at 10:19
     and Kathryn gets on.
   > At some point, Luca must have bought
     the flight ticket from Fiftyville to Dallas.
   > Kathryn takes the flight, which departs
     at 17:20.

   QUESTIONS:
   > Money withdrawal from ATM on Leggett Street?
   --> Let's have a look. */

/* TASK 10 - Fetch information about Kathryn's and Luca's
   bank account history. We only need their ID numbers. */
SELECT QUOTE('');
SELECT QUOTE('TASK 10 - BANK ACCOUNTS OWNED BY KATHRYN AND/OR LUCA');
SELECT account_number FROM bank_accounts WHERE person_id = 561160;
SELECT account_number FROM bank_accounts WHERE person_id = 467400;

/* RESULTS:
   > Kathryn has got NO bank account.
   > Luca has got ONE bank account.
   --> ID of account: 28500762.

   QUESTION:
   > How on Earth coul Kathryn get money from an ATM
     without having a bank account? */

/* TASK 11 - Fetch ATM transactions on Leggett Street on 2021-07-28. */

SELECT QUOTE('');
SELECT QUOTE('TASK 11 - FIND ATM TRANSACTIONS ON LEGGETT STREET');
SELECT QUOTE('          ON 2021-07-28 AND IN RELATION TO LUCA\'S');
SELECT QUOTE('          ACCOUNT');
SELECT account_number,year,month,day,atm_location,transaction_type,amount FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND account_number = 28500762;

/* RESULTS:
   > FOUND withdrawal of 48 dollars.
   > THE QUESTION BEFORE TASK 11 REMAINS UNANSWERED */


/************************************************/
/* ATTEMPT 1: FAILURE                           */
/* REASON - It makes no sense to withdraw money */
/* from an ATM without having a bank account    */
/* THIEF LEFT FIFTYVILLE ON 2021-07-29, NOT 28! */
/************************************************/
