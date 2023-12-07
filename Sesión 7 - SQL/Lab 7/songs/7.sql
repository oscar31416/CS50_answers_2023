/* Get average energy of Drake's songs. */
.open songs.db
SELECT AVG(energy) FROM songs WHERE duration_ms > 0 AND artist_id = (SELECT id FROM artists WHERE name = 'Drake');
