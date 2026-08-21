use ratatui::{
    layout::{Constraint, Direction, Layout, Rect},
    style::{Color, Modifier, Style},
    text::{Line, Span},
    widgets::{Block, Borders, Gauge, List, ListItem, Paragraph},
    Frame,
};

use crate::app::App;

pub fn draw(f: &mut Frame, app: &mut App) {
    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .constraints([
            Constraint::Length(3), // header: now playing
            Constraint::Min(3),    // playlist
            Constraint::Length(3), // footer: status + volume
        ])
        .split(f.area());

    draw_header(f, app, chunks[0]);
    draw_playlist(f, app, chunks[1]);
    draw_footer(f, app, chunks[2]);
}

fn draw_header(f: &mut Frame, app: &App, area: Rect) {
    let current = app
        .playlist
        .current()
        .map(|s| s.title)
        .unwrap_or_else(|_| "—".to_string());

    let (state_text, state_color) = if app.playlist.current().is_err() {
        ("■ Stopped", Color::DarkGray)
    } else if app.player.is_paused() {
        ("⏸ Paused", Color::Yellow)
    } else {
        ("▶ Playing", Color::Green)
    };

    let elapsed = format_duration(app.player.elapsed());

    let line = Line::from(vec![
        Span::styled(
            state_text,
            Style::default().fg(state_color).add_modifier(Modifier::BOLD),
        ),
        Span::raw("  "),
        Span::styled(
            current,
            Style::default().fg(Color::Cyan).add_modifier(Modifier::BOLD),
        ),
        Span::raw(format!("   {elapsed}")),
    ]);

    let block = Block::default()
        .borders(Borders::ALL)
        .title(" Terminal Music Player ");

    f.render_widget(Paragraph::new(line).block(block), area);
}

fn draw_playlist(f: &mut Frame, app: &mut App, area: Rect) {
    let items: Vec<ListItem> = app
        .playlist
        .iter_with_marker()
        .into_iter()
        .enumerate()
        .map(|(i, (is_current, song))| {
            let marker = if is_current { "▶ " } else { "  " };
            let text = format!("{marker}{:>2}. {}", i + 1, song.title);
            let style = if is_current {
                Style::default().fg(Color::Green).add_modifier(Modifier::BOLD)
            } else {
                Style::default()
            };
            ListItem::new(text).style(style)
        })
        .collect();

    let list = List::new(items)
        .block(
            Block::default()
                .borders(Borders::ALL)
                .title(format!(" Playlist ({}) ", app.playlist.len())),
        )
        .highlight_style(
            Style::default()
                .bg(Color::DarkGray)
                .add_modifier(Modifier::BOLD),
        )
        .highlight_symbol("➤ ");

    f.render_stateful_widget(list, area, &mut app.list_state);
}

fn draw_footer(f: &mut Frame, app: &App, area: Rect) {
    let chunks = Layout::default()
        .direction(Direction::Horizontal)
        .constraints([Constraint::Percentage(72), Constraint::Percentage(28)])
        .split(area);

    let help =
        "↑/↓ select · Enter play · Space pause · n/p next/prev · +/- vol · q quit";
    let status_line = format!("{}  |  {}", app.status, help);

    let status = Paragraph::new(status_line).block(Block::default().borders(Borders::ALL).title(" Status "));
    f.render_widget(status, chunks[0]);

    // volume is stored 0.0..=2.0 internally (rodio allows amplification above 1.0);
    // display it as a 0-100% bar over that same range.
    let vol_pct = ((app.player.volume() / 2.0) * 100.0).clamp(0.0, 100.0) as u16;
    let gauge = Gauge::default()
        .block(Block::default().borders(Borders::ALL).title(" Volume "))
        .gauge_style(Style::default().fg(Color::Magenta))
        .percent(vol_pct);
    f.render_widget(gauge, chunks[1]);
}

fn format_duration(d: std::time::Duration) -> String {
    let total = d.as_secs();
    format!("{:02}:{:02}", total / 60, total % 60)
}