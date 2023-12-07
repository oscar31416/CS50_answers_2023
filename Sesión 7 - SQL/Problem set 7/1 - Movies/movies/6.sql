/* Average rating of all films released in 2012 */
.open movies.db
SELECT AVG(rating) FROM ratings WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);
