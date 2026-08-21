use std::path::{Path, PathBuf};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Song {
    pub title: String,
    pub path: PathBuf,
}

impl Song {
    pub fn from_path(path: PathBuf) -> Self {
        let title = path
            .file_stem()
            .map(|s| s.to_string_lossy().to_string())
            .unwrap_or_else(|| "Unknown Title".to_string());

        Song { title, path }
    }

    pub fn extension_is_supported(path: &Path) -> bool {
        const SUPPORTED: [&str; 4] = ["mp3", "wav", "flac", "ogg"];
        path.extension()
            .and_then(|e| e.to_str())
            .map(|ext| SUPPORTED.contains(&ext.to_lowercase().as_str()))
            .unwrap_or(false)
    }
}

impl std::fmt::Display for Song {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.title)
    }
}
