use crate::xe_string_2_rust_string::{XeString, rust_string_to_xe_string, xe_string_to_rust_string};
use std::{ffi::c_char, ptr};

fn set_config(xe_str_directory: *const c_char , xe_str_name: *const c_char, mut pxe_str_extensions: *const *const c_char, extensions_number: usize) -> rfd::FileDialog
{
    let mut fd = rfd::FileDialog::new();
    if xe_str_directory != ptr::null()
    {
        let rust_str_directory = xe_string_to_rust_string(xe_str_directory);
        fd = fd.set_directory(rust_str_directory);
    }

    if extensions_number != 0 || pxe_str_extensions != ptr::null()
    {
        let mut extension_list: Vec<_> = Vec::new();
        let rust_str_name= xe_string_to_rust_string(xe_str_name);
        let mut i = extensions_number;

        while i != 0
        {
            let rust_str_extansion = xe_string_to_rust_string(unsafe { ptr::read(pxe_str_extensions) });
            extension_list.push(rust_str_extansion);

            pxe_str_extensions = unsafe { pxe_str_extensions.add(1) };
            i -=1;
        }
        fd = fd.add_filter(rust_str_name, &extension_list);
    }
    return fd;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn rfd_save_file(xe_str_directory: *const c_char , xe_str_name: *const c_char, pxe_str_extensions: *const *const c_char, extensions_number: usize) -> XeString
{
    let out_xe_str = XeString
    {
        data : ptr::null(),
        size : 0
    };
    let out_rust_path;
    let path_buf_out: std::path::PathBuf;

    let fd= set_config(xe_str_directory, xe_str_name, pxe_str_extensions, extensions_number);
    match fd.save_file()
    {
        Some(val)=> path_buf_out = val,
        None=>return out_xe_str,
    }

    match path_buf_out.to_str() 
    {
        Some(val)   => out_rust_path = String::from(val),
        None=>return out_xe_str,
    }
    return rust_string_to_xe_string(out_rust_path);
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn rfd_pick_file(xe_str_directory: *const c_char , xe_str_name: *const c_char, pxe_str_extensions: *const *const c_char, extensions_number: usize) -> XeString
{
    let out_xe_str = XeString
    {
        data : ptr::null(),
        size : 0
    };
    let out_rust_path;
    let path_buf_out: std::path::PathBuf;

    let fd= set_config(xe_str_directory, xe_str_name, pxe_str_extensions, extensions_number);
    match fd.pick_file()
    {
        Some(val)=> path_buf_out = val,
        None=>return out_xe_str,
    }

    match path_buf_out.to_str() 
    {
        Some(val)   => out_rust_path = String::from(val),
        None=>return out_xe_str,
    }
    return rust_string_to_xe_string(out_rust_path);
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn rfd_pick_folder(xe_str_directory: *const c_char , xe_str_name: *const c_char, pxe_str_extensions: *const *const c_char, extensions_number: usize) -> XeString
{
    let out_xe_str = XeString
    {
        data : ptr::null(),
        size : 0
    };
    let out_rust_path;
    let path_buf_out: std::path::PathBuf;

    let fd= set_config(xe_str_directory, xe_str_name, pxe_str_extensions, extensions_number);
    match fd.pick_folder()
    {
        Some(val)=> path_buf_out = val,
        None=>return out_xe_str,
    }

    match path_buf_out.to_str() 
    {
        Some(val)   => out_rust_path = String::from(val),
        None=>return out_xe_str,
    }
    return rust_string_to_xe_string(out_rust_path);
}