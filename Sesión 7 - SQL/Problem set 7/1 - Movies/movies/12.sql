/* Get the titles of all films in which both Bradley Cooper and Jennifer Lawrence starred. */
.open movies.db
SELECT DISTINCT(title) FROM movies WHERE id IN (SELECT movie_id FROM stars WHERE person_id IN (SELECT id FROM people WHERE (name = 'Bradley Cooper' OR name = 'Jennifer Lawrence')));
