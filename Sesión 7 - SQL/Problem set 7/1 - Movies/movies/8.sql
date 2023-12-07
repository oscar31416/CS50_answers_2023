/* Get the names of all people who starred in Toy Story. */
.open movies.db
SELECT name FROM people WHERE id IN (SELECT person_id FROM stars WHERE movie_id IN (SELECT id FROM movies where title = 'Toy Story'));
