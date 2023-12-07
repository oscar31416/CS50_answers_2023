/* Average energy of all songs */
.open songs.db
SELECT AVG(energy) FROM songs WHERE duration_ms > 0;
