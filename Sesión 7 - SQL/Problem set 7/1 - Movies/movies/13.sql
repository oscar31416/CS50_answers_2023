/* Get the names of people who starred in films in which the person
   whose name is Kevin Bacon and whose birth year was 1958 also starred.
   Exclude that particular Kevin Bacon from the list. */

.open movies.db

/* The ID of that particular Kevin Bacon is fetched this way:

>>>SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958;

   Kevin's ID is 102. Now let's fetch the IDs of all films in which
   people.id = 102 (the Kevin Bacon that was born in 1958) starred.

>>>SELECT movie_id FROM stars WHERE person_id IN (people.id = 102);

   PLEASE, the text between brackets shall NOT be considered actual
   SQLite3 code but just a way of summing up the first command, which
   is what must actually go between the brackets.

   Let's name the resulting column of data 'films_kb58'. The next step
   is fetching the IDs of the people who starred in those films
   EXCLUDING Kevin's and avoiding repetitions.

>>>SELECT DISTINCT(person_id) FROM stars WHERE movie_id IN (films_kb58) AND person_id <> (people.id = 102);

   If we name the resulting list 'people_kb58', we can finally get
   the names of those people.

>>>SELECT name FROM people WHERE id IN (people_kb58); */
SELECT name FROM people WHERE id IN (SELECT DISTINCT(person_id) FROM stars WHERE movie_id IN (SELECT movie_id FROM stars WHERE person_id IN (SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958)) AND person_id <> (SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958));
