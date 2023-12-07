/* Suppose that 'songs.db' gathers a listener's top 100 songs from 2018.
   Get the average energy, valence and danceability */
.open songs.db
--Average energy
SELECT QUOTE('Average energy:');
SELECT AVG(energy) FROM songs WHERE duration_ms > 0;
--Average valence
SELECT QUOTE('Average valence:');
SELECT AVG(valence) FROM songs WHERE duration_ms > 0;
--Average danceability
SELECT QUOTE('Average danceability:');
SELECT AVG(danceability) FROM songs WHERE duration_ms > 0;
