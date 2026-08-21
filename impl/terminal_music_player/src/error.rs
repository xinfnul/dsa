use std::path::PathBuf;
use thiserror::Error;

#[derive(Debug, Error)]
pub enum PlayerError {
    #[error("no audio output device on this system: {0}")]
    NoOutputDevice(String),

    #[error("failed to open audio file '{path}': {source}")]
    FileOpen {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },

    #[error("failed to decode audio file '{path}': {message}")]
    Decode { path: PathBuf, message: String },
}

#[derive(Debug, Error)]
pub enum PlaylistError {
    #[error("playlist is empty")]
    Empty,

    #[error("no directory entry could be read: {0}")]
    DirRead(#[from] std::io::Error),

    #[error("no supported audio files ( mp3/wav/flac/ogg ) found in '{0}'")]
    NoTracksFound(PathBuf),

    #[error("index {index} out of bounds ( playlist has {len} tracks )")]
    IndexOutOfBounds { index: usize, len: usize },
}

#[derive(Debug, Error)]
pub enum AppError {
    #[error(transparent)]
    Player(#[from] PlayerError),

    #[error(transparent)]
    Playlist(#[from] PlaylistError),

    #[error(transparent)]
    Io(#[from] std::io::Error),
}
