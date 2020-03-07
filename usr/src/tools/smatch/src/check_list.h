#ifndef CK
#define CK(_x) void _x(int id);
#define __undo_CK_def
#endif

CK(register_db_call_marker) /* always has to be first  */
CK(register_param_used)     /* get_state_hooks have to be registered before smatch_extra */
CK(register_container_of)
CK(register_container_of2)
CK(register_smatch_extra)   /* smatch_extra always has to be SMATCH_EXTRA */
CK(register_smatch_extra_links)
CK(register_modification_hooks)
/*
 * Implications should probably be after all the modification and smatch_extra
 * hooks have run.
 *
 */
CK(register_implications)
CK(register_definition_db_callbacks)
CK(register_project)
CK(register_untracked_param)
CK(register_buf_comparison)
CK(register_buf_comparison_links)
CK(register_param_compare_limit)
CK(register_param_compare_limit_links)
CK(register_returns_early)

CK(register_param_cleared)  /* param_set relies on param_cleared */
CK(register_param_set)

CK(register_smatch_ignore)
CK(register_buf_size)
CK(register_strlen)
CK(register_strlen_equiv)
CK(register_capped)
CK(register_parse_call_math)
CK(register_param_filter)
CK(register_struct_assignment)
CK(register_comparison)
CK(register_comparison_links)
CK(register_comparison_inc_dec)
CK(register_comparison_inc_dec_links)
CK(register_function_ptrs)
CK(register_annotate)
CK(register_start_states)
CK(register_type_val)
CK(register_data_source)
CK(register_common_functions)
CK(register_function_info)
CK(register_type_links)
CK(register_impossible)
CK(register_impossible_return)
CK(register_strings)
CK(register_integer_overflow)
CK(register_integer_overflow_links)
CK(register_real_absolute)
CK(register_imaginary_absolute)
CK(register_bits)
CK(register_fn_arg_link)
CK(register_parameter_names)
CK(register_param_limit)
CK(register_return_to_param)
CK(register_return_to_param_links)
CK(register_constraints)
CK(register_constraints_required)
CK(register_about_fn_ptr_arg)
CK(register_mtag)
CK(register_mtag_map)
CK(register_mtag_data)
CK(register_param_to_mtag_data)
CK(register_array_values)
CK(register_nul_terminator)
CK(register_nul_terminator_param_set)
CK(register_statement_count)
CK(register_fresh_alloc)

CK(register_kernel_user_data)
CK(register_kernel_user_data2)

CK(check_debug)

CK(check_bogus_loop)

CK(check_deref)
CK(check_check_deref)
CK(check_dereferences_param)
CK(check_index_overflow)
CK(check_index_overflow_loop_marker)
CK(check_testing_index_after_use)
CK(check_memcpy_overflow)
CK(check_strcpy_overflow)
CK(check_sprintf_overflow)
CK(check_snprintf_overflow)
CK(check_allocating_enough_data)
CK(check_leaks)
CK(check_type)
CK(check_allocation_funcs)
CK(check_frees_argument)
CK(check_deref_check)
CK(check_signed)
CK(check_precedence)
CK(check_unused_ret)
CK(check_dma_on_stack)
CK(check_param_mapper)
CK(check_call_tree)
CK(check_dev_queue_xmit)
CK(check_stack)
CK(check_no_return)
CK(check_mod_timer)
CK(check_return)
CK(check_resource_size)
CK(check_release_resource)
CK(check_proc_create)
CK(check_freeing_null)
CK(check_frees_param)
CK(check_free)
CK(check_frees_param_strict)
CK(check_free_strict)
CK(check_no_effect)
CK(check_kunmap)
CK(check_snprintf)
CK(check_macros)
CK(check_return_efault)
CK(check_gfp_dma)
CK(check_unwind)
CK(check_kmalloc_to_bugon)
CK(check_platform_device_put)
CK(check_info_leak)
CK(check_return_enomem)
CK(check_get_user_overflow)
CK(check_get_user_overflow2)
CK(check_access_ok_math)
CK(check_container_of)
CK(check_input_free_device)
CK(check_select)
CK(check_memset)
CK(check_logical_instead_of_bitwise)
CK(check_kmalloc_wrong_size)
CK(check_pointer_math)
CK(check_bit_shift)
CK(check_macro_side_effects)
CK(check_sizeof)
CK(check_return_cast)
CK(check_or_vs_and)
CK(check_passes_sizeof)
CK(check_assign_vs_compare)
CK(check_missing_break)
CK(check_array_condition)
CK(check_struct_type)
CK(check_64bit_shift)
CK(check_wrong_size_arg)
CK(check_cast_assign)
CK(check_readl_infinite_loops)
CK(check_double_checking)
CK(check_shift_to_zero)
CK(check_indenting)
CK(check_unreachable)
CK(check_no_if_block)
CK(check_buffer_too_small_for_struct)
CK(check_uninitialized)
CK(check_signed_integer_overflow_check)
CK(check_continue_vs_break)
CK(check_impossible_mask)
CK(check_syscall_arg_type)
CK(check_trinity_generator)

/* <- your test goes here */
/* CK(register_template) */

/* kernel specific */
CK(check_kernel_printf)
CK(check_locking)
CK(check_puts_argument)
CK(check_err_ptr)
CK(check_err_ptr_deref)
CK(check_expects_err_ptr)
CK(check_held_dev)
CK(check_return_negative_var)
CK(check_rosenberg)
CK(check_rosenberg2)
CK(check_rosenberg3)
CK(check_wait_for_common)
CK(check_bogus_irqrestore)
CK(check_zero_to_err_ptr)
CK(check_freeing_devm)
CK(check_off_by_one_relative)
CK(check_capable)
CK(check_ns_capable)
CK(check_test_bit)
CK(check_dma_mapping_error)
CK(check_nospec)
CK(check_nospec_barrier)
CK(check_spectre)
CK(check_spectre_second_half)
CK(check_implicit_dependencies)

/* wine specific stuff */
CK(check_wine_filehandles)
CK(check_wine_WtoA)

CK(check_arm64_tagged)

/* illumos specific */
CK(check_all_func_returns)
CK(check_cmn_err)

#include "check_list_local.h"

CK(register_scope)
CK(register_stored_conditions)
CK(register_stored_conditions_links)
CK(register_parsed_conditions)
CK(register_sval)
CK(register_buf_size_late)
CK(register_smatch_extra_late)
CK(register_assigned_expr) /* This is used by smatch_extra.c so it has to come really late */
CK(register_assigned_expr_links)
CK(register_modification_hooks_late)  /* has to come after smatch_extra */
CK(register_comparison_late) /* has to come after modification_hooks_late */
CK(register_function_hooks)
CK(check_kernel)  /* this is overwriting stuff from smatch_extra_late */
CK(check_wine)
CK(register_returns)

#ifdef __undo_CK_def
#undef CK
#undef __undo_CK_def
#endif
