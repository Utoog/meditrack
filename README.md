# meditrack

<a href="https://www.hannahilea.com/blog/houseplant-programming">
  <img alt="Static Badge" src="https://img.shields.io/badge/%F0%9F%AA%B4%20Houseplant%20-x?style=flat&amp;label=Project%20type&amp;color=1E1E1D">
</a>

MediTrack is a tool to keep track of your finished/planned/abandoned movies, games, tv series and etc.

# Commands

- add - Create new media entry
- edit - Edit a media entry (TODO)
- remove - Remove a media entry
- list - Print all media entries (filters are WIP)
- show - Print specific media entry information
- search - Search media which contains <text> in its name

# Example

```
$ meditrack add -t MV -n "Very Nice Movie" -y 1997
Media added succesfully!
$ meditrack list
[MV] Cool movie (2012) (id: 1)
	Status: Dropped		Rating: -
[MV] Great Movie (1999) (id: 2)
	Status: Finished		Rating: +
[MV] IDK Movie (2017) (id: 3)
	Status: Planned		Rating: /
[MV] Bad Movie (2007) (id: 4)
	Status: Dropped		Rating: -
[VG] Awesome Game (2011) (id: 5)
	Status: In Progress		Rating: +
[VG] Awesome Game: Prequel (1998) (id: 6)
	Status: In Progress		Rating: +
[VG] Awesome Game: Prequel Sequel (1999) (id: 7)
	Status: Planned		Rating: /
[TV] The TV Show (2016) (id: 8)
	Status: Finished		Rating: +
[MUS] Some Tunes (2017) (id: 9)
	Status: Finished		Rating: +
[MUS] More Tunes (1997) (id: 10)
	Status: Dropped		Rating: /
[MV] Very Nice Movie (1997) (id: 11)
	Status: Planned		Rating: /
```
