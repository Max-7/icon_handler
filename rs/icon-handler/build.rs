fn main() {
    // Use our own def file
    println!("cargo:rustc-link-arg=/DEF library.def");
}
