/* Get names of songs that feature other artists.
   They include 'feat.' on their name. */
.open songs.db
-- 'LOCATE' is not part of SQLite but 'INSTR' is.
SELECT name FROM songs WHERE duration_ms > 0 AND INSTR(name, 'feat') <> 0;
