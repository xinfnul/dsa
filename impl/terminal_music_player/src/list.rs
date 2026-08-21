#![allow(dead_code)]

use std::{
    cell::RefCell,
    rc::{Rc, Weak},
};

type Link<T> = Option<Rc<RefCell<Node<T>>>>;
type PrevLink<T> = Option<Weak<RefCell<Node<T>>>>;

struct Node<T> {
    data: T,
    next: Link<T>,
    prev: PrevLink<T>,
}

pub struct List<T> {
    head: Link<T>,
    tail: Link<T>,
    len: usize,
}

impl<T> List<T> {
    pub fn new() -> Self {
        Self {
            head: None,
            tail: None,
            len: 0,
        }
    }

    pub fn push_front(&mut self, value: T) {
        let new_node = Rc::new(RefCell::new(Node {
            data: value,
            prev: None,
            next: self.head.clone(),
        }));

        match self.head.take() {
            Some(old_head) => {
                old_head.borrow_mut().prev = Some(Rc::downgrade(&new_node));
                self.head = Some(new_node);
            }

            None => {
                self.tail = Some(new_node.clone());
                self.head = Some(new_node);
            }
        }

        self.len += 1;
    }

    pub fn push_back(&mut self, value: T) {
        let new_node = Rc::new(RefCell::new(Node {
            data: value,
            next: None,
            prev: self.tail.as_ref().map(Rc::downgrade),
        }));

        match self.tail.take() {
            Some(old_tail) => {
                old_tail.borrow_mut().next = Some(new_node.clone());
                self.tail = Some(new_node);
            }

            None => {
                self.head = Some(new_node.clone());
                self.tail = Some(new_node);
            }
        }

        self.len += 1;
    }

    pub fn pop_front(&mut self) -> bool {
        match self.head.take() {
            Some(old_head) => {
                let next = old_head.borrow_mut().next.take();

                match next {
                    Some(new_head) => {
                        new_head.borrow_mut().prev = None;
                        self.head = Some(new_head);
                    }
                    None => {
                        self.tail = None;
                    }
                }

                self.len -= 1;
                true
            }
            None => false,
        }
    }

    pub fn pop_back(&mut self) -> bool {
        match self.tail.take() {
            Some(old_tail) => {
                let prev = old_tail.borrow_mut().prev.clone();

                match prev {
                    Some(prev_weak) => {
                        if let Some(new_tail) = prev_weak.upgrade() {
                            new_tail.borrow_mut().next = None;
                            self.tail = Some(new_tail);
                        }
                    }
                    None => {
                        self.head = None;
                    }
                }

                self.len -= 1;
                true
            }
            None => false,
        }
    }

    pub fn insert_at(&mut self, index: usize, value: T) -> bool {
        if index > self.len {
            return false;
        }

        if index == 0 {
            self.push_front(value);
            return true;
        }

        if index == self.len {
            self.push_back(value);
            return true;
        }

        let current = self.node_at(index).unwrap();

        let previous = current
            .borrow()
            .prev
            .as_ref()
            .and_then(|weak| weak.upgrade())
            .unwrap();

        let new_node = Rc::new(RefCell::new(Node {
            data: value,
            prev: Some(Rc::downgrade(&previous)),
            next: Some(current.clone()),
        }));

        previous.borrow_mut().next = Some(new_node.clone());
        current.borrow_mut().prev = Some(Rc::downgrade(&new_node));

        self.len += 1;

        true
    }

    pub fn remove_at(&mut self, index: usize) -> bool {
        if index >= self.len {
            return false;
        }

        if index == 0 {
            return self.pop_front();
        }

        if index == self.len - 1 {
            return self.pop_back();
        }

        let node = self.node_at(index).unwrap();

        let (previous, next) = {
            let node_ref = node.borrow();

            let previous = node_ref
                .prev
                .as_ref()
                .and_then(|weak| weak.upgrade())
                .unwrap();

            let next = node_ref.next.clone().unwrap();

            (previous, next)
        };

        previous.borrow_mut().next = Some(next.clone());
        next.borrow_mut().prev = Some(Rc::downgrade(&previous));

        self.len -= 1;

        true
    }

    pub fn remove_value(&mut self, value: &T) -> bool
    where
        T: PartialEq,
    {
        let mut current = self.head.clone();

        while let Some(node) = current {
            let matches = {
                let node_ref = node.borrow();
                node_ref.data == *value
            };

            if matches {
                let is_head = self
                    .head
                    .as_ref()
                    .map_or(false, |head| Rc::ptr_eq(head, &node));

                let is_tail = self
                    .tail
                    .as_ref()
                    .map_or(false, |tail| Rc::ptr_eq(tail, &node));

                if is_head {
                    return self.pop_front();
                }

                if is_tail {
                    return self.pop_back();
                }

                let (previous, next) = {
                    let node_ref = node.borrow();

                    let previous = node_ref
                        .prev
                        .as_ref()
                        .and_then(|weak| weak.upgrade())
                        .unwrap();

                    let next = node_ref.next.clone().unwrap();

                    (previous, next)
                };

                previous.borrow_mut().next = Some(next.clone());
                next.borrow_mut().prev = Some(Rc::downgrade(&previous));

                self.len -= 1;

                return true;
            }

            current = node.borrow().next.clone();
        }

        false
    }

    pub fn clear(&mut self) {
        self.head = None;
        self.tail = None;
        self.len = 0;
    }

    pub fn reverse(&mut self) {
        let mut current = self.head.clone();

        while let Some(node) = current {
            let next = {
                let mut node_ref = node.borrow_mut();

                let old_next = node_ref.next.clone();

                node_ref.next = node_ref.prev.as_ref().and_then(|weak| weak.upgrade());

                node_ref.prev = old_next.as_ref().map(Rc::downgrade);

                old_next
            };

            current = next;
        }

        std::mem::swap(&mut self.head, &mut self.tail);
    }

    pub fn find(&self, value: &T) -> Option<usize>
    where
        T: PartialEq,
    {
        let mut current = self.head.clone();
        let mut index = 0;

        while let Some(node) = current {
            let node_ref = node.borrow();

            if node_ref.data == *value {
                return Some(index);
            }

            current = node_ref.next.clone();
            index += 1;
        }

        None
    }

    pub fn at(&self, index: usize) -> Option<T>
    where
        T: Clone,
    {
        let node = self.node_at(index)?;

        Some(node.borrow().data.clone())
    }

    fn node_at(&self, index: usize) -> Link<T> {
        if index >= self.len {
            return None;
        }

        if index <= self.len / 2 {
            let mut current = self.head.clone();

            for _ in 0..index {
                let next = current.as_ref().and_then(|node| node.borrow().next.clone());

                current = next;
            }

            current
        } else {
            let mut current = self.tail.clone();

            for _ in 0..(self.len - 1 - index) {
                let previous = current
                    .as_ref()
                    .and_then(|node| node.borrow().prev.as_ref().and_then(|weak| weak.upgrade()));

                current = previous;
            }

            current
        }
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn len(&self) -> usize {
        self.len
    }
}

impl<T: std::fmt::Display> std::fmt::Display for List<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut current = self.head.clone();

        while let Some(node) = current {
            let node_ref = node.borrow();

            write!(f, "{}", node_ref.data)?;

            current = node_ref.next.clone();

            if current.is_some() {
                write!(f, " <-> ")?;
            }
        }

        Ok(())
    }
}

// ----------------------------------------------------------------------------------

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn new_list_is_empty() {
        let list: List<i32> = List::new();

        assert!(list.is_empty());
        assert_eq!(list.len(), 0);
        assert_eq!(list.at(0), None);
    }

    #[test]
    fn push_front_adds_in_correct_order() {
        let mut list = List::new();

        list.push_front(1000);
        list.push_front(100);
        list.push_front(10);

        assert_eq!(list.len(), 3);
        assert_eq!(list.at(0), Some(10));
        assert_eq!(list.at(1), Some(100));
        assert_eq!(list.at(2), Some(1000));
    }

    #[test]
    fn push_back_adds_in_correct_order() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);
        list.push_back(1000);

        assert_eq!(list.len(), 3);
        assert_eq!(list.at(0), Some(10));
        assert_eq!(list.at(1), Some(100));
        assert_eq!(list.at(2), Some(1000));
    }

    #[test]
    fn push_front_and_push_back_work_together() {
        let mut list = List::new();

        list.push_back(100);
        list.push_front(10);
        list.push_back(1000);
        list.push_front(0);

        assert_eq!(list.len(), 4);
        assert_eq!(list.at(0), Some(0));
        assert_eq!(list.at(1), Some(10));
        assert_eq!(list.at(2), Some(100));
        assert_eq!(list.at(3), Some(1000));
    }

    #[test]
    fn pop_front_removes_first_element() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);
        list.push_back(1000);

        assert!(list.pop_front());
        assert_eq!(list.len(), 2);
        assert_eq!(list.at(0), Some(100));
        assert_eq!(list.at(1), Some(1000));

        assert!(list.pop_front());
        assert!(list.pop_front());

        assert!(list.is_empty());
        assert_eq!(list.len(), 0);
    }

    #[test]
    fn pop_back_removes_last_element() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);
        list.push_back(1000);

        assert!(list.pop_back());
        assert_eq!(list.len(), 2);
        assert_eq!(list.at(0), Some(10));
        assert_eq!(list.at(1), Some(100));

        assert!(list.pop_back());
        assert!(list.pop_back());

        assert!(list.is_empty());
        assert_eq!(list.len(), 0);
    }

    #[test]
    fn popping_empty_list_returns_false() {
        let mut list: List<i32> = List::new();

        assert!(!list.pop_front());
        assert!(!list.pop_back());
        assert_eq!(list.len(), 0);
    }

    #[test]
    fn insert_at_beginning_middle_and_end() {
        let mut list = List::new();

        list.push_back(0);
        list.push_back(1000);

        assert!(list.insert_at(1, 10));
        assert!(list.insert_at(2, 100));
        assert!(list.insert_at(4, 10000));

        assert_eq!(list.len(), 5);
        assert_eq!(list.at(0), Some(0));
        assert_eq!(list.at(1), Some(10));
        assert_eq!(list.at(2), Some(100));
        assert_eq!(list.at(3), Some(1000));
        assert_eq!(list.at(4), Some(10000));
    }

    #[test]
    fn insert_at_invalid_index_returns_false() {
        let mut list = List::new();

        list.push_back(0);
        list.push_back(1000);

        assert!(!list.insert_at(3, 98));
        assert_eq!(list.len(), 2);
        assert_eq!(list.at(0), Some(0));
        assert_eq!(list.at(1), Some(1000));
    }

    #[test]
    fn remove_at_beginning_and_end() {
        let mut list = List::new();

        for value in 1..=5 {
            list.push_back(value);
        }

        assert!(list.remove_at(1));
        assert_eq!(list.at(0), Some(1));

        assert!(list.remove_at(2));
        assert_eq!(list.at(0), Some(1));
        assert_eq!(list.at(1), Some(3));

        assert!(list.remove_at(list.len() - 1));
        assert_eq!(list.at(0), Some(1));
        assert_eq!(list.at(1), Some(3));
    }

    #[test]
    fn remove_at_invalid_index_returns_false() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);

        assert!(!list.remove_at(3));
        assert_eq!(list.len(), 2);
        assert_eq!(list.at(0), Some(10));
        assert_eq!(list.at(1), Some(100));
    }

    #[test]
    fn remove_value_removes_first_match() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);
        list.push_back(1000);

        assert!(list.remove_value(&1000));

        assert_eq!(list.len(), 2);
        assert_eq!(list.at(0), Some(10));
        assert_eq!(list.at(1), Some(100));
    }

    #[test]
    fn remove_value_handles_head_and_tail() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);
        list.push_back(1000);

        assert!(list.remove_value(&10));
        assert_eq!(list.at(0), Some(100));

        assert!(list.remove_value(&1000));
        assert_eq!(list.at(0), Some(100));

        assert_eq!(list.len(), 1);
    }

    #[test]
    fn remove_missing_value_returns() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);
        list.push_back(1000);

        assert!(!list.remove_value(&98));

        assert_eq!(list.len(), 3);
        assert_eq!(list.at(0), Some(10));
        assert_eq!(list.at(1), Some(100));
        assert_eq!(list.at(2), Some(1000));
    }

    #[test]
    fn find_returns_first_matching_index() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);
        list.push_back(1000);

        assert_eq!(list.find(&10), Some(0));
        assert_eq!(list.find(&100), Some(1));
        assert_eq!(list.find(&1000), Some(2));
        assert_eq!(list.find(&98), None);
    }

    #[test]
    fn at_returns_none_for_invalid_index() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);

        assert_eq!(list.at(0), Some(10));
        assert_eq!(list.at(1), Some(100));
        assert_eq!(list.at(3), None);
        assert_eq!(list.at(usize::MAX), None);
    }

    #[test]
    fn clear_empties_the_list() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);

        list.clear();

        assert!(list.is_empty());
        assert_eq!(list.len(), 0);
        assert_eq!(list.at(0), None);

        list.push_back(98);

        assert_eq!(list.len(), 1);
        assert_eq!(list.at(0), Some(98));
    }

    #[test]
    fn reverse_changes_element_order() {
        let mut list = List::new();

        list.push_back(10);
        list.push_back(100);

        list.reverse();

        assert_eq!(list.len(), 2);
        assert_eq!(list.at(0), Some(100));
        assert_eq!(list.at(1), Some(10));
    }

    #[test]
    fn display_formats_list_correctly() {
        let mut list = List::new();

        list.push_back(1);
        list.push_back(3);
        list.push_back(5);
        list.push_back(7);
        list.push_back(9);

        assert_eq!(format!("{}", list), "1 <-> 3 <-> 5 <-> 7 <-> 9");
    }
}
