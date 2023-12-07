/* Get the title and release year of all Harry Potter films. */
.open movies.db
SELECT title,year FROM movies WHERE INSTR(title, 'Harry Potter') = 1 ORDER BY year;
