use std::{fs::File, path::Path, time::Duration};

use rodio::{Decoder, DeviceSinkBuilder, MixerDeviceSink, Player as RodioPlayer};

use crate::error::PlayerError;

pub struct Player {
    stream: MixerDeviceSink,
    player: Option<RodioPlayer>,
    volume: f32,
}

impl Player {
    pub fn new() -> Result<Self, PlayerError> {
        let stream = DeviceSinkBuilder::open_default_sink()
            .map_err(|e| PlayerError::NoOutputDevice(e.to_string()))?;

        Ok(Self {
            stream,
            player: None,
            volume: 1.0,
        })
    }

    pub fn play(&mut self, path: &Path) -> Result<(), PlayerError> {
        let file = File::open(path).map_err(|source| PlayerError::FileOpen {
            path: path.to_path_buf(),
            source,
        })?;

        let source = Decoder::try_from(file).map_err(|e| PlayerError::Decode {
            path: path.to_path_buf(),
            message: e.to_string(),
        })?;

        // Drop the old player first ( stops the previous track ).
        self.player = None;

        let player = RodioPlayer::connect_new(self.stream.mixer());

        player.set_volume(self.volume);
        player.append(source);
        player.play();

        self.player = Some(player);

        Ok(())
    }

    pub fn pause(&self) {
        if let Some(player) = &self.player {
            player.pause();
        }
    }

    pub fn resume(&self) {
        if let Some(player) = &self.player {
            player.play();
        }
    }

    pub fn stop(&mut self) {
        self.player = None;
    }

    pub fn set_volume(&mut self, volume: f32) {
        self.volume = volume.clamp(0.0, 2.0);

        if let Some(player) = &self.player {
            player.set_volume(self.volume)
        }
    }

    pub fn volume(&self) -> f32 {
        self.volume
    }

    pub fn is_paused(&self) -> bool {
        self.player
            .as_ref()
            .map(|player| player.is_paused())
            .unwrap_or(false)
    }

    pub fn is_finished(&self) -> bool {
        self.player
            .as_ref()
            .map(|player| player.empty())
            .unwrap_or(false)
    }

    pub fn elapsed(&self) -> Duration {
        self.player
            .as_ref()
            .map(|player| player.get_pos())
            .unwrap_or_default()
    }
}
