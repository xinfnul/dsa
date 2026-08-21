use std::path::PathBuf;

use ratatui::widgets::ListState;

use crate::{error::AppError, player::Player, playlist::PlayList};

pub struct App {
    pub playlist: PlayList,
    pub player: Player,
    pub list_state: ListState,
    pub status: String,
    pub should_quit: bool,
}

impl App {
    pub fn new(dir: PathBuf) -> Result<Self, AppError> {
        let playlist = PlayList::from_directory(&dir)?;
        let mut player = Player::new()?;

        let mut list_state = ListState::default();
        list_state.select(Some(0));

        let status = match playlist.current() {
            Ok(song) => {
                player.play(&song.path)?;
                "Playing".to_string()
            }
            Err(_) => "Playlist empty".to_string(),
        };

        Ok(Self {
            playlist,
            player,
            list_state,
            status,
            should_quit: false,
        })
    }

    /// Called every event-loop tick.
    pub fn on_tick(&mut self) {
        if self.player.is_finished() && !self.player.is_paused() {
            match self.playlist.advance_on_finish() {
                Ok(song) => match self.player.play(&song.path) {
                    Ok(()) => {
                        self.sync_selection();
                        self.status = "Playing".to_string();
                    }
                    Err(e) => self.status = format!("error: {e}"),
                },
                Err(_) => self.status = "Stopped (end of playlist)".to_string(),
            }
        }
    }

    /// Keep the highlighted row following whichever the track is currently playing.
    fn sync_selection(&mut self) {
        let index = self
            .playlist
            .iter_with_marker()
            .iter()
            .position(|(is_current, _)| *is_current);

        self.list_state.select(index);
    }

    pub fn move_selection(&mut self, delta: i32) {
        let len = self.playlist.len();

        if len == 0 {
            return;
        }

        let current = self.list_state.selected().unwrap_or(0) as i32;
        let next = (current + delta).rem_euclid(len as i32) as usize;

        self.list_state.select(Some(next));
    }

    /// Play whatever row is currently highlighted ( Enter key ).
    pub fn play_selected(&mut self) {
        let Some(i) = self.list_state.selected() else {
            return;
        };

        match self.playlist.jump_to(i) {
            Ok(song) => match self.player.play(&song.path) {
                Ok(()) => self.status = "Playing".to_string(),
                Err(e) => self.status = format!("error: {e}"),
            },
            Err(e) => self.status = format!("error: {e}"),
        }
    }

    pub fn next(&mut self) {
        match self.playlist.next() {
            Ok(song) => {
                if let Err(e) = self.player.play(&song.path) {
                    self.status = format!("error: {e}");
                } else {
                    self.sync_selection();
                    self.status = "Playing".to_string();
                }
            }
            Err(e) => self.status = format!("error: {e}"),
        }
    }

    pub fn prev(&mut self) {
        match self.playlist.prev() {
            Ok(song) => {
                if let Err(e) = self.player.play(&song.path) {
                    self.status = format!("error: {e}");
                } else {
                    self.sync_selection();
                    self.status = "Playing".to_string();
                }
            }
            Err(e) => self.status = format!("error: {e}"),
        }
    }

    pub fn toggle_pause(&mut self) {
        if self.playlist.current().is_err() {
            return;
        }
        if self.player.is_paused() {
            self.player.resume();
            self.status = "Playing".to_string();
        } else {
            self.player.pause();
            self.status = "Paused".to_string();
        }
    }

    pub fn volume_up(&mut self) {
        self.player.set_volume(self.player.volume() + 0.1);
    }

    pub fn volume_down(&mut self) {
        self.player.set_volume(self.player.volume() - 0.1);
    }
}
