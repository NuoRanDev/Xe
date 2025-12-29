use std::{ffi::{CStr, CString, c_char}, ptr};

pub struct XeString
{
    pub(crate) size : i64,
    pub(crate) data : *const c_char,
}

pub fn rust_string_to_xe_string(rust_str:String) -> XeString 
{
    let size_u8= rust_str.len() as i64;
    let c_string;
    let mut xe_str:XeString = XeString
    {
        size : 0,
        data : ptr::null(),
    };

    match CString::new(rust_str)
    {
        Ok(c_string_result) => c_string = c_string_result,
        Err(err) => return xe_str,
    }

    xe_str = XeString
    {
        size : size_u8,
        data : c_string.into_raw(), // Move ownership to C
    };
    return xe_str;
}

pub fn xe_string_to_rust_string(xe_str: *const c_char) -> String
{
    let c_string =  unsafe { CStr::from_ptr(xe_str) };
    let rust_str: String = String::from("");
    match c_string.to_str()
    {
        Ok(c_string_result) => return c_string_result.to_owned(),
        Err(err) => return rust_str,
    }
}

/// # Safety
/// The ptr should be a valid pointer to the string allocated by rust
#[unsafe(no_mangle)]
pub unsafe extern "C" fn free_rust_string(ptr: *const c_char) 
{
    // Take the ownership back to rust and drop the owner
    let _ = unsafe { CString::from_raw(ptr as *mut _) };
}