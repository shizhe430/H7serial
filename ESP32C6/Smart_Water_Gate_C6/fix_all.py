import re

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# 1. Revert sim_cmd_handler to use queue (no MQTT at all)
old_sim = '''    int fp_id = atoi(argv[1]);
    int vol = (argc >= 3) ? atoi(argv[2]) : 0;
    if (fp_id > 0) current_user_id = fp_id;
    else current_user_id = -1;
    printf(">>> User %d", fp_id > 0 ? fp_id : 0);
    if (vol > 0) {
        water_volume += vol;
        last_dispensed_volume = vol;
        user_record_t *u = find_user_by_id(current_user_id);
        if (u) {
            u->daily_water += vol;
            u->use_count++;
            u->total_water += vol;
        }
        printf(", water %dml", vol);
        save_users_to_nvs();
        save_users_full();
        update_all_params();
    }
    printf("\\n");
    return 0;
}'''

new_sim = '''    int fp_id = atoi(argv[1]);
    int vol = (argc >= 3) ? atoi(argv[2]) : 0;
    if (fp_id > 0) {
        current_user_id = fp_id;
        printf(">>> User %d", fp_id);
    }
    if (vol > 0) {
        water_volume += vol;
        last_dispensed_volume = vol;
        user_record_t *u = find_user_by_id(current_user_id);
        if (u) {
            u->daily_water += vol;
            u->use_count++;
            u->total_water += vol;
        }
        printf(", water %dml", vol);
        save_users_to_nvs();
        save_users_full();
    }
    /* MQTT update via dispense_worker task */
    if (vol > 0 && dispense_queue) {
        dispense_event_t ev = {.vol = vol, .fp_id = current_user_id, .set_fp = 1};
        xQueueSend(dispense_queue, &ev, 0);
    } else {
        update_all_params();
    }
    printf("\\n");
    return 0;
}'''

content = content.replace(old_sim, new_sim)
print("1. Fixed sim_cmd_handler")

# 2. Change update_all_params to call MQTT directly (not through work queue)
old_up = '''static void update_all_params(void)
{
    esp_rmaker_work_queue_add_task(_rmaker_update, NULL);
}'''

new_up = '''static void update_all_params(void)
{
    _rmaker_update(NULL);
}'''

content = content.replace(old_up, new_up)
print("2. Fixed update_all_params")

# 3. Change dispense_worker to ONLY do MQTT update, not duplicate the data processing
old_dw = '''                user_record_t *u = find_user_by_id(current_user_id);
                water_valve_open = false;
                water_volume += ev.vol;
                last_dispensed_volume = ev.vol;
                if (u) u->use_count++;
                if (last_vol_param) esp_rmaker_param_update_and_report(last_vol_param, esp_rmaker_int(ev.vol));

                /* Call notify (NVS + UART + queues MQTT via work queue) */
                notify_daily_water(ev.vol);'''

new_dw = '''                /* Data already saved by sim/STM32 handler - just push MQTT update */
                update_all_params();'''

content = content.replace(old_dw, new_dw)
print("3. Fixed dispense_worker")

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
