:- module(main, [is_movie_directed_by/2, total_awards_nominated/2, all_movies_directed_by/2, total_movies_released_in/3, all_movies_released_between/4]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

is_movie_directed_by(Title,Director):- movie(Title,Director,_,_,_,_).

total_awards_nominated(Title,Nominations):-
    movie(Title,_,_,OscarNoms,EmmyNoms,GoldenGlobeNoms),
    Nominations is OscarNoms + EmmyNoms + GoldenGlobeNoms.

all_films_directed_by_given_director(Director,MovieList):-
    findall(Title,movie(Title,Director,_,_,_,_),MovieList).

all_movies_directed_by(Director,Movies):-
    all_films_directed_by_given_director(Director,Movies).

total_movies_released_in([],_,0).
total_movies_released_in([HeadMovie|T],Year,Count):-
    movie(HeadMovie,_,Year,_,_,_),
    total_movies_released_in(T,Year,RestCount),
    Count is RestCount + 1.
total_movies_released_in([HeadMovie|T],Year,Count):-
    movie(HeadMovie,_,DifferentYear,_,_,_),
    Year \= DifferentYear,
    total_movies_released_in(T,Year,Count).


all_movies_released_between([],_,_,[]).
all_movies_released_between([HeadMovie|T],MinYear,MaxYear,MoviesBetween):-
    movie(HeadMovie,_,Year,_,_,_),
    all_movies_released_between(T,MinYear,MaxYear,MoviesBetween),
    Year <  MinYear.
all_movies_released_between([HeadMovie|T],MinYear,MaxYear,MoviesBetween):-
    movie(HeadMovie,_,Year,_,_,_),
    all_movies_released_between(T,MinYear,MaxYear,MoviesBetween),
    Year >  MaxYear.
all_movies_released_between([HeadMovie|T],MinYear,MaxYear,[HeadMovie|RestMoviesBetween]):-
    movie(HeadMovie,_,Year,_,_,_),
    all_movies_released_between(T,MinYear,MaxYear,RestMoviesBetween),
    Year =< MaxYear,
    Year >= MinYear.