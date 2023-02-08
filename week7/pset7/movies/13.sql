SELECT DISTINCT(people.name) FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE movies.id IN
    (
        SELECT movie_id FROM movies
        JOIN stars on movies.id = stars.movie_id
        JOIN people ON stars.person_id = people.id
        WHERE people.name = 'Kevin Bacon' AND birth = 1958
    )
    AND NOT people.name = 'Kevin Bacon';