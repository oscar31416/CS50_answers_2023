/* Get songs with danceability, energy
   and valence greater than 0.75 */
.open songs.db
SELECT name FROM songs WHERE duration_ms > 0 AND danceability > 0.75 AND energy > 0.75 AND valence > 0.75;
