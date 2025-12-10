use std::{ffi::{CString,c_char}, ptr, str::FromStr};

pub struct xe_string
{
    pub(crate) size:i64,
    pub(crate) data:*const c_char,
}

pub fn create_xe_string(rust_str:String) -> xe_string 
{
    let size_u8= rust_str.len() as i64;
    let c_string = CString::new(rust_str).expect("THIS XE RUST ERROR");
    let xe_str:xe_string = xe_string
    {
        size : size_u8,
        data : c_string.into_raw(), // Move ownership to C
    };
    return xe_str;
}

/// # Safety
/// The ptr should be a valid pointer to the string allocated by rust
#[unsafe(no_mangle)]
pub unsafe extern "C" fn free_string(ptr: *const c_char) {
    // Take the ownership back to rust and drop the owner
    let _ = CString::from_raw(ptr as *mut _);
}

pub struct filter_name
{
    pub(crate) text_brief:String,
    pub(crate) file_type:String,
}

#[unsafe(no_mangle)]
pub extern "C" fn save_file() -> xe_string
{
    let path_buf_out: std::path::PathBuf;
    let out_rust_path;

    let out_xe_str = xe_string
    {
        data : ptr::null(),
        size : 0
    };

    let fd: rfd::FileDialog = rfd::FileDialog::new()

    .set_file_name("foo.txt")
    .set_directory("&path");
    
    match fd.save_file()
    {
        Some(val)=> path_buf_out = val,
        None=>return out_xe_str,
    }

    match path_buf_out.to_str() 
    {
        Some(val)   => out_rust_path = String::from_str(val).expect("CString::new failed"),
        None=>return out_xe_str,
    }
    return create_xe_string(out_rust_path);
}