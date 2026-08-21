mod list;

use crate::list::List;

fn main() {
    let mut list = List::new();

    list.push_back(10);
    list.push_back(100);
    list.push_back(1000);

    list.push_front(0);

    list.insert_at(1, 1);

    list.remove_value(&1);

    println!("{list}");

    println!("length = {}", list.len());
}
