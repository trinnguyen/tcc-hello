use structopt::StructOpt;
use std::str::Chars;
use std::fs::File;
use std::io::Read;

#[derive(Debug, StructOpt)]
struct Cli {
    file: String,
}

struct Input<'a> {
    chars: Chars<'a>,
    look_ahead: Option<char>,
}

impl Input<'_> {
    fn next(&mut self) -> Option<char> {
        if let Some(sc) = self.look_ahead {
            self.look_ahead = None;
            return Some(sc)
        }

        return self.chars.next();
    }

    fn set_look_ahead(&mut self, c: char) {
        self.look_ahead = Some(c);
    }
}

fn main() -> () {
    let opt = Cli::from_args();
    let filename = opt.file;
    println!("source file: {:?}", filename);

    // read content
    if let Ok(mut f) = File::open(filename) {
        let mut src = String::new();
        if let Err(_) = f.read_to_string(&mut src) {
            return;
        }
        
        println!("{}", src);

        // run scan
        let mut input = Input {
            chars: src.chars(),
            look_ahead: None
        };
        scan(&mut input);
    }
}

fn scan(input: &mut Input) {
    println!("start scanning...");
    loop {
        let tok = scan_next(input);
        match tok {
            Token::Eof => break,
            t => println!("new tok: {:?}", t),
        }
    }
}

fn scan_next(input: &mut Input) -> Token {

    // skip whilespace
    let mut c = input.next();
    while is_whitespace(&c) {
        c = input.next();
    }

    // check invalid
    if let Some(sc) = c {
        match sc {
            // check symbol
            ';' => Token::Semicolon,
            ',' => Token::Comma,
            '(' => Token::ParentOpen,
            ')' => Token::ParentClose,
            '{' => Token::BracketOpen,
            '}' => Token::BracketClose,
            t if t.is_ascii_alphabetic() => scan_keywords_or_id(&t, input),
            t if t.is_numeric() => scan_const_int(&t, input),
            _ => Token::Eof
        }
    } else {
        return Token::Eof;
    }
}

fn scan_keywords_or_id(t: &char, input: &mut Input) -> Token {
    let mut s = String::from("");
    s.push(t.clone());

    // go next
    let mut c = input.next();
    loop {
        if let Some(sc) = c {
            if sc.is_ascii_alphanumeric() {
                s.push(sc.clone());
                c = input.next();
            } else {
                input.set_look_ahead(sc);
                break;
            }
        }
    }

    // return
    return match s.as_str() {
        "int" => Token::Int,
        "void" => Token::Void,
        "return" => Token::Return,
        _ => Token::Id(s)
    }
}

fn scan_const_int(t: &char, input: &mut Input) -> Token {
    let mut s = String::from("");
    s.push(t.clone());

    // go next
    let mut c = input.next();
    loop {
        if let Some(sc) = c {
            if sc.is_numeric() {
                s.push(sc.clone());
                c = input.next();
            } else {
                input.set_look_ahead(sc);
                break;
            }
        }
    }

    // parse
    let val : i32 = s.parse().unwrap();
    return Token::ConstInt(val)
}

fn is_whitespace(c: &Option<char>) -> bool {
    if let Some(sc) = c {
        return sc.is_ascii_whitespace();
    } else {
        return false;
    }
}

#[derive(Debug)]
enum Token {
    // keywords
    Int,
    Void,
    Return,

    // ID
    Id(String),
    Eof,

    // symbols
    ParentOpen,
    ParentClose,
    BracketOpen,
    BracketClose,
    Comma,
    Semicolon,

    // Number
    ConstInt(i32)
}