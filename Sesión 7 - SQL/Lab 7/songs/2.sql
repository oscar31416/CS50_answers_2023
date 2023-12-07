/* TASK - Sort songs by increasing order of tempo. */
-- Open data file.
.open songs.db
-- Sort by increasing tempo.
-- The 'WHERE' condition is there to avoid something strange.
SELECT name FROM songs WHERE duration_ms > 0 ORDER BY tempo
