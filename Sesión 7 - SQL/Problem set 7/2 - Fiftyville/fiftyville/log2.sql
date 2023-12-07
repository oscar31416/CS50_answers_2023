/* STUDENT'S NOTE:
   - DATE FORMAT: YYYY-MM-DD
   - TIME FORMAT: hh:mm (24-hour format) */

/**************************************************/
/* GOALS:                                         */
/* > Name of thief?                               */
/* > Thief took a flight afterwards. Destination? */
/* > Accomplice? If any, name(s)?                 */
/*                                                */
/* STARTING POINT:                                */
/* > Place of theft: Humphrey Street.             */
/* > Date of theft: 2021-07-28.                   */
/**************************************************/

/*************/
/* ATTEMPT 2 */
/*************/

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 1:
--Fetch data about robberies on Humphrey Street on 2021-07-28.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT id,year,month,day,street,description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day BETWEEN 25 AND 31;

--RESULTS:
--> CS50 duck stolen on 2021-07-28 at 10:15 at the Humphrey Street bakery.
--> Three interviews with transcripts available.
----> They all mention the bakery.

--QUESTIONS:
--> Information about transcripts?

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 2:
--Fetch information from transcripts on 2021-07-28 about robbery.
--I'll get interviews between 2021-07-25 and 2021-07-31...
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT id,year,month,day,name,transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day BETWEEN 25 AND 31;

--RESULTS:
--> Ruth - Within 10 minutes after theft, bad guy got on a car in
--  the car park and drove away. Suggested looking at records.
--> Eugene - Thief spotted taking money from ATM on Leggett Street
--  some time before the event.
--> Raymond - Thief talks to someone on the phone after robbery.
--  Less than a minute. PLAN: LEAVE FIFTYVILLE ON 2021-07-29
--  AS SOON AS POSSIBLE.
--  Bad guy asks person to buy flight ticket.

--QUESTIONS:
--> This is a big one: IS ANYONE LYING???
--> Did the thief themself drive or did they have someone do it?

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 3:
--Start by fetching number plates of cars
--that left the car park between 10:10 and 10:59.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT hour,minute,license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 10 AND activity = 'exit';

--RESULTS:
--> 9 number plates:
----> 5P2BI95 10:16
----> 94KL13X 10:18
----> 6P58WS2 10:18
----> 4328GD8 10:19
----> G412CB7 10:20
----> L93JTIZ 10:21
----> 322W7JE 10:23
----> 0NTHK55 10:23
----> 1106N58 10:35 >>> DISCARDED

--DISCARDED SUBJECTS: the one with number plate 1106N58.
--> Reason: left outide expected time interval.

--QUESTIONS:
--> To whom do those cars belong?

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 4:
--Fetch data about owners of cars.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT id,name,phone_number,passport_number,license_plate FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 10 AND minute < 30 AND activity = 'exit');

--RESULTS:
-->     ID |    Name |   Phone number | Passport number | Number plate
--> 221103 | Vanessa | (725) 555-4692 |      2963008352 | 5P2BI95
--> 243696 | Barry   | (301) 555-4174 |      7526138472 | 6P58WS2
--> 396669 | Iman    | (829) 555-5269 |      7049073643 | L93JTIZ
--> 398010 | Sofia   | (130) 555-0289 |      1695452385 | G412CB7
--> 467400 | Luca    | (389) 555-5198 |      8496433585 | 4328GD8
--> 514354 | Diana   | (770) 555-1861 |      3592750733 | 322W7JE
--> 560886 | Kelsey  | (499) 555-9472 |      8294398571 | 0NTHK55
--> 686048 | Bruce   | (367) 555-5533 |      5773159633 | 94KL13X

--QUESTIONS:
--> Who took money from an ATM on Leggett Street on 2021-07-28?

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 5:
--Fetch withdrawals on Leggett Street on 2021-07-28.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT id,account_number,amount FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';

--RESULTS:
-->  ID |  Account | Quantity
--> 246 | 28500762 | 48
--> 264 | 28296815 | 20
--> 266 | 76054385 | 60
--> 267 | 49610011 | 50
--> 269 | 16153065 | 80
--> 288 | 25506511 | 20
--> 313 | 81061156 | 30
--> 336 | 26013199 | 35

--QUESTIONS:
--> To whom do these accounts belong?

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 6:
--Fetch information about bank accounts from TASK 5.
--To be more accurate, OWNER of each account.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT person_id,account_number FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw');

--RESULTS:
-->     ID |  Account
--> 686048 | 49610011
--> 514354 | 26013199
--> 458378 | 16153065
--> 395717 | 28296815
--> 396669 | 25506511
--> 467400 | 28500762
--> 449774 | 76054385
--> 438727 | 81061156

--Suspect chart now looks like this:
-->     ID |    Name |   Phone number | Passport number | Number plate | Bank account number
--> 221103 | Vanessa | (725) 555-4692 |      2963008352 |      5P2BI95 |                   ?
--> 243696 |   Barry | (301) 555-4174 |      7526138472 |      6P58WS2 |                   ?
--> 396669 |    Iman | (829) 555-5269 |      7049073643 |      L93JTIZ |            25506511
--> 398010 |   Sofia | (130) 555-0289 |      1695452385 |      G412CB7 |                   ?
--> 467400 |    Luca | (389) 555-5198 |      8496433585 |      4328GD8 |            28500762
--> 514354 |   Diana | (770) 555-1861 |      3592750733 |      322W7JE |            26013199
--> 560886 |  Kelsey | (499) 555-9472 |      8294398571 |      0NTHK55 |                   ?
--> 686048 |   Bruce | (367) 555-5533 |      5773159633 |      94KL13X |            49610011

--DISCARDED SUSPECTS: Vanessa, Barry, Sofia, Kelsey.
--> None of them withdrew money from Leggett Street on 2021-07-28.

--QUESTIONS:
--> Raymond mentioned a conversation on the phone.
----> To whom did the suspects talk on the phone on 2021-07-28?

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 7:
--Fetch call records for all suspects on 2021-07-28.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT caller,receiver,duration FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60;

--RESULTS:
--> Iman is discarded: no phone calls.
--> Luca is discarded: we're looking for the caller.
--> REMAINING SUSPECTS: Diana, Bruce.

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 8:
--Fetch receivers of calls from suspects.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT id,name,phone_number,passport_number,license_plate FROM people
WHERE phone_number = '(725) 555-3243' OR phone_number = '(375) 555-8161';

--RESULTS:
-->     ID |   Name |   Phone number | Passport number | Number plate
--> 847116 | Philip | (725) 555-3243 |      3391710505 |      GW362R6
--> 864400 |  Robin | (375) 555-8161 |   (No passport) |      4V16VO0

--CONCLUSIONS:
--> Either Diana and Philip form the thief-accomplice combo,
--  or Bruce and Robin are the baddies.

--SITUATION UPDATE - CURRENT SUSPECTS
--(NO DISTINCTION BETWEEN THIEF AND ACCOMPLICE):
-->     ID |   Name |   Phone number | Passport number | Number plate | Bank account number
--> 514354 |  Diana | (770) 555-1861 |      3592750733 |      322W7JE |            26013199
--> 847116 | Philip | (725) 555-3243 |      3391710505 |      GW362R6
--> 686048 |  Bruce | (367) 555-5533 |      5773159633 |      94KL13X |            49610011
--> 864400 |  Robin | (375) 555-8161 |   (No passport) |      4V16VO0

--QUESTIONS:
--> Diana, Philip, Bruce or Robin. One of them took the first flight
--  that departed from Fiftyville on 2021-07-29, but... WHO?
--> Where did the thief go?

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 9:
--Fetch information about first flight that left Fiftyville on 2021-07-29.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT id FROM airports WHERE city = 'Fiftyville';
SELECT id,year,month,day,hour,minute,origin_airport_id,destination_airport_id FROM flights
WHERE origin_airport_id = 8 AND year = 2021 AND month = 7 AND day = 29;

--RESULTS:
--> Fiftyville airport ID: 8.
--> Date and time of flight: 2021-07-29 08:20.
--> Flight ID: 36.
--> Destination airport ID: 4.
--> Destination city: New York City.

--QUESTIONS:
--> Who took that flight?

--vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
--TASK 10:
--Fetch information about flight with ID = 36.
--Find what suspects (if any) took the flight.
--^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SELECT passport_number FROM passengers WHERE flight_id = 36 AND
(passport_number = 3592750733 OR passport_number = 3391710505 OR passport_number = 5773159633);

--RESULTS:
--> ONE MATCH: BRUCE

/*******************************************/
/* CONCLUSIONS:                            */
/* > Bruce stole the CS50 duck.            */
/* > Bruce took a flight to New York City. */
/* > His accomplice is Robin.              */
/*******************************************/
