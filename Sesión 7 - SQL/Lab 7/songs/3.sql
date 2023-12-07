/* List the names of the five longest songs.
   Sort by decreasing length. */
-- Load file
.open songs.db
-- 
SELECT name FROM songs WHERE duration_ms > 0 ORDER BY duration_ms DESC LIMIT 5;
