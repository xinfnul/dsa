use std::{fs, path::Path};

use crate::{error::PlaylistError, list::List, song::Song};

pub struct PlayList {
    list: List<Song>,
    current_index: Option<usize>,
}

impl PlayList {
    pub fn new() -> Self {
        Self {
            list: List::new(),
            current_index: None,
        }
    }

    /// Scan a directory ( non-recursive ) for suported audio files and build a playlist.
    pub fn from_directory(dir: &Path) -> Result<Self, PlaylistError> {
        let mut paths: Vec<_> = fs::read_dir(dir)?
            .filter_map(|entry| entry.ok())
            .map(|entry| entry.path())
            .filter(|p| p.is_file() && Song::extension_is_supported(p))
            .collect();

        paths.sort();

        if paths.is_empty() {
            return Err(PlaylistError::NoTracksFound(dir.to_path_buf()));
        }

        let mut playlist = PlayList::new();
        for path in paths {
            playlist.add(Song::from_path(path));
        }

        Ok(playlist)
    }

    pub fn add(&mut self, song: Song) {
        self.list.push_back(song);

        if self.current_index.is_none() {
            self.current_index = Some(0);
        }
    }

    pub fn is_empty(&self) -> bool {
        self.list.is_empty()
    }

    pub fn len(&self) -> usize {
        self.list.len()
    }

    pub fn current(&self) -> Result<Song, PlaylistError> {
        let index = self.current_index.ok_or(PlaylistError::Empty)?;

        self.list.at(index).ok_or(PlaylistError::IndexOutOfBounds {
            index,
            len: self.list.len(),
        })
    }

    pub fn jump_to(&mut self, index: usize) -> Result<Song, PlaylistError> {
        if index >= self.list.len() {
            return Err(PlaylistError::IndexOutOfBounds {
                index,
                len: self.list.len(),
            });
        }

        self.current_index = Some(index);
        self.current()
    }

    pub fn next(&mut self) -> Result<Song, PlaylistError> {
        let index = self.current_index.ok_or(PlaylistError::Empty)?;

        if index + 1 < self.list.len() {
            self.current_index = Some(index + 1);
        }

        self.current()
    }

    pub fn prev(&mut self) -> Result<Song, PlaylistError> {
        let index = self.current_index.ok_or(PlaylistError::Empty)?;

        if index > 0 {
            self.current_index = Some(index - 1);
        }

        self.current()
    }

    /// Called when the currently loaded track finishes playing naturally
    pub fn advance_on_finish(&mut self) -> Result<Song, PlaylistError> {
        let index = self.current_index.ok_or(PlaylistError::Empty)?;

        if index + 1 < self.list.len() {
            self.current_index = Some(index + 1);
            self.current()
        } else {
            Err(PlaylistError::Empty)
        }
    }

    /// Returns `(is_current, song)` pairs in playlist order, for display.
    pub fn iter_with_marker(&self) -> Vec<(bool, Song)> {
        (0..self.list.len())
            .filter_map(|i| {
                self.list
                    .at(i)
                    .map(|song| (Some(i) == self.current_index, song))
            })
            .collect()
    }
}
