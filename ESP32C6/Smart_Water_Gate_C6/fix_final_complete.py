with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# 1. simplify sim_cmd_handler - just store data and print, NO MQTT
old_sim = '''    int fp_id = atoi(argv[1]);
    int vol = (argc >= 3) ? atoi(argv[2]) : 0;
    if (fp_id > 0) current_user_id = fp_id;
    else current_user_id = -1;
    printf(">>> User %d", fp_id > 0 ? fp_id : 0);
    if (vol > 0 && dispense_queue) {
        dispense_event_t ev = {.vol = vol, .fp_id = current_user_id, .set_fp = 1};
        xQueueSend(dispense_queue, &ev, 0);
        printf(", water %dml (processing)", vol);
    }
    printf("\\n");
    return 0;
}'''

new_sim = '''    int fp_id = atoi(argv[1]);
    int vol = (argc >= 3) ? atoi(argv[2]) : 0;
    if (fp_id > 0) current_user_id = fp_id;
    else current_user_id = -1;

    if (vol > 0) {
        water_volume += vol;
        last_dispensed_volume = vol;
        user_record_t *u = find_user_by_id(current_user_id);
        if (u) {
            u->daily_water += vol;
            u->use_count++;
            u->total_water += vol;
        }
    }
    printf(">>> User %d, water %dml\\n", fp_id > 0 ? fp_id : 0, vol);
    return 0;
}'''

content = content.replace(old_sim, new_sim)

# 2. restore work queue for update_all_params
old_up = '''static void update_all_params(void)
{
    _rmaker_update(NULL);
}'''

new_up = '''static void update_all_params(void)
{
    esp_rmaker_work_queue_add_task(_rmaker_update, NULL);
}'''

content = content.replace(old_up, new_up)

# 3. keep dispense_worker as before (for STM32 use)
# (should already be correct from fix_all2.py)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
