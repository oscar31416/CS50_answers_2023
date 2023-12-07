/* Get names of people who starred in a film that was released in 2004.
   Sort by birth year, if it is known.
   No need to sort the names alphabetically.
   Each name should appear in the list only once. */
.open movies.db
SELECT DISTINCT(name) FROM people JOIN stars ON people.id = stars.person_id JOIN movies ON movies.id = stars.movie_id WHERE movies.year = 2004 ORDER BY people.birth;
