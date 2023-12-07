/* Get the titles of songs by Post Malone. */
.open songs.db
SELECT name FROM songs WHERE duration_ms > 0 AND artist_id = (SELECT id FROM artists WHERE name = 'Post Malone');
