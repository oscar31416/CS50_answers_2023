/* Get number of films with a rating of 10.0 */
.open movies.db
SELECT COUNT(title) FROM movies WHERE id = (SELECT movie_id FROM ratings WHERE rating = 10);
