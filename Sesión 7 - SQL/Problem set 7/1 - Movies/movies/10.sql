/* Get names of directors one of whose films got a rating of, at least, 9.0.
   Each name should appear just once. */
.open movies.db
SELECT DISTINCT(name) FROM people WHERE id IN (SELECT person_id FROM directors WHERE movie_id IN (SELECT movie_id FROM ratings WHERE rating >= 9.0));
