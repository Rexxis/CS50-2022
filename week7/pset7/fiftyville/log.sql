-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT * FROM crime_scene_reports; -- check entry on crime_scene_reports


-- read crime scene report from the day of incident
SELECT * FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28;


-- incident occured on 10:15 at Humphrey Street Bakery


-- read interviews from day of incident with mentions of bakery
SELECT id, transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';


-- incident interviews in id 161, 162 and 163


-- check bakery security logs at day of incident to identify thief's car
SELECT hour, minute, activity, license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28;


-- identified 8 cars exited the premises within 10 minutes after the incident
SELECT hour, minute, activity, license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 AND minute <= 25;

-- following up witness interview 162


-- check atm transactions data on day of incident
SELECT * FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28;


-- narrow down atm transactions only in leggett street and withdraw
SELECT * FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';


-- check bank account names with the activity matches in the interview for potential suspects
SELECT people.name, phone_number, passport_number, license_plate FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number in
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
    );


-- narrow down the searches with license plate that exited the bakery
SELECT people.name, phone_number, passport_number, license_plate FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number in
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
    )
    and license_plate in
    (
        SELECT license_plate FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 and minute <= 25
    );


-- we have 4 suspects


-- follow up interview 163


-- check phone calls on the day and hour of the incident
SELECT caller, receiver, duration FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28;


-- narrow down the search where the duration less than 1 minute
SELECT caller, receiver, duration FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;


-- cross check the phone number with the suspect list
SELECT people.name, phone_number, passport_number, license_plate FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number in
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
    )
    and license_plate in
    (
        SELECT license_plate FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 and minute <= 25
    )
    and phone_number in
    (
        SELECT caller FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
    );


-- narrowed down to 2 suspects


-- peek inside airports table
SELECT * FROM airports;


-- get the flights that leaves fiftyville on 29 July 2021
SELECT * FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2021 AND month = 7 AND day = 29;


-- get passport number that leaves CSF on 29 July 2021
SELECT passport_number FROM passengers
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2021 AND month = 7 AND day = 29;


-- cross check the suspect list with the passport number
SELECT people.name, phone_number, passport_number, license_plate FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number in
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
    )
    and license_plate in
    (
        SELECT license_plate FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 and minute <= 25
    )
    and phone_number in
    (
        SELECT caller FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
    )
    and passport_number in
    (
        SELECT passport_number FROM passengers
        JOIN flights ON passengers.flight_id = flights.id
        JOIN airports ON flights.origin_airport_id = airports.id
        WHERE year = 2021 AND month = 7 AND day = 29 AND abbreviation = 'CSF'
    );


-- suspect list still 2


-- check the first flight on 29 July 2021
SELECT * FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute;


-- cross check suspect list with passport number that departs earliest flight on 29 July 2021
SELECT people.name, people.phone_number, people.passport_number, people.license_plate FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour;


-- cross check first flight with suspect list
SELECT people.name, phone_number, passport_number, license_plate FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE account_number in
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
    )
    and license_plate in
    (
        SELECT license_plate FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 and minute <= 25
    )
    and phone_number in
    (
        SELECT caller FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
    )
    and passport_number in
    (
        SELECT passport_number FROM passengers
        JOIN flights ON passengers.flight_id = flights.id
        JOIN airports ON flights.origin_airport_id = airports.id
        WHERE year = 2021 AND month = 7 AND day = 29 AND hour = 8 AND minute = 20
    );

-- suspect identified as Bruce

-- getting the accomplish name
SELECT people.name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE caller = '(367) 555-5533' AND year = 2021 AND month = 7 AND day = 28 AND duration < 60;

-- identified Robin from Bruce call receiver

-- getting city destination from the first flight
SELECT city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour LIMIT 1;
