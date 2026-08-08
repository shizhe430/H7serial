import re

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

old = '''static int sim_cmd_handler(int argc, char **argv)
{
    if (argc < 2) {
        puts("Usage: sim <fp_id> <vol_ml>");
        puts("  sim 1 200  - User1 + 200ml");
        puts("  sim 0 150  - Guest + 150ml");
    }
    int fp_id = atoi(argv[1]);
    if (fp_id > 0) current_user_id = fp_id;
    else current_user_id = -1;
    int vol = (argc >= 3) ? atoi(argv[2]) : 0;
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
        printf(", %dml", vol);
    }
    printf("\\n");
    return 0;
}'''

new = '''static int sim_cmd_handler(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: sim <fp_id> <vol_ml>\\n");
        printf("  sim 1 200  - User1 + 200ml\\n");
        printf("  sim 0 150  - Guest + 150ml\\n");
        return 0;
    }
    int fp_id = atoi(argv[1]);
    int vol = (argc >= 3) ? atoi(argv[2]) : 0;
    if (fp_id > 0) current_user_id = fp_id;
    if (vol > 0 && dispense_queue) {
        dispense_event_t ev = {.vol = vol, .fp_id = current_user_id, .set_fp = 1};
        xQueueSend(dispense_queue, &ev, 0);
    }
    printf(">>> Queued: user %d, %dml\\n", fp_id > 0 ? fp_id : 0, vol);
    return 0;
}'''

if old in content:
    content = content.replace(old, new)
    with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("REPLACED")
else:
    print("NOT FOUND")
    # debug
    idx = content.find('sim_cmd_handler')
    if idx > 0:
        print("Found at:", idx)
        print(repr(content[idx:idx+600]))
