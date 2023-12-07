/* Open the database file. */
.open songs.db

/* The file contains two tables: 'songs' and 'artists'.
   From 'songs', we need the items from the column 'name'.

   The WHERE condition shouldn't have been necessary,
   but I had to include it in order to avoid an error. */
SELECT name FROM songs WHERE duration_ms > 0;
