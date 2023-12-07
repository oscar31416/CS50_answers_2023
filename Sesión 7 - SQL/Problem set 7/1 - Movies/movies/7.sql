/* List all films released in 2010 and sort by decreasing rating.
   For films with the same rating, sort alphabetically by title.
   Expected two columns: one for title, and one for rating.
   Exclude films without rating. */
.open movies.db
SELECT title, rating FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE movies.year = 2010 ORDER BY rating DESC, title;
