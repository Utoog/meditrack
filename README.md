# meditrack

Meditrack is a tool to keep track of your finished/planned/abandoned movies, games, tv series and etc.

Program uses sqlite3 for database handling, file should be located at `$HOME/.local/share/meditrack`

# Commands

- `add` - Create new media entry
- `edit` - Edit a media entry (TODO)
- `remove` - Remove a media entry
- `purge` - Remove every entry from the database
- `list` - Print all media entries (TODO: filters)
- `show` - Print specific media entry information
- `search` - Search media

# TODO

- `status` command to easily change specified media status and rating

# Example

```
$ meditrack add -t mv -n "Very Nice Movie" -y 1997
Media added succesfully!
$ meditrack list
[MV] Cool Movie (2012) (id: 1)
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
