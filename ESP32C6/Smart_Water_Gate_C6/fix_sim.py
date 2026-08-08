import re

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

old = '''    int fp_id = atoi(argv[1]);
    int vol = (argc >= 3) ? atoi(argv[2]) : 0;
    if (fp_id > 0) current_user_id = fp_id;
    else current_user_id = -1;
    printf(">>> User %d", fp_id > 0 ? fp_id : 0);
    if (vol > 0) {
        dispense_event_t ev = {.vol = vol, .fp_id = current_user_id, .set_fp = 1};
        if (dispense_queue) xQueueSend(dispense_queue, &ev, 0);
        printf(", water %dml", vol);
    }
    printf("\\n");
    return 0;
}'''

new = '''    int fp_id = atoi(argv[1]);
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

content = content.replace(old, new)
with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
