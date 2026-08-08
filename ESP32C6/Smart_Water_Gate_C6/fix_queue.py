with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

old = '''static void update_all_params(void)
{
    user_record_t *u = find_user_by_id(current_user_id);
    int uid = (current_user_id == FP_UNKNOWN) ? 0 : current_user_id;
    if (user_id_param) esp_rmaker_param_update_and_report(user_id_param, esp_rmaker_int(uid));
    if (last_vol_param) esp_rmaker_param_update_and_report(last_vol_param, esp_rmaker_int(last_dispensed_volume));
    if (count_param) esp_rmaker_param_update_and_report(count_param, esp_rmaker_int(u ? u->use_count : 0));
    int daily = u ? u->daily_water : 0;
    if (daily_param) esp_rmaker_param_update_and_report(daily_param, esp_rmaker_int(daily));
    int target = calculate_daily_target(u);
    int remain = target - daily;
    if (remain < 0) remain = 0;
    if (remain_param) esp_rmaker_param_update_and_report(remain_param, esp_rmaker_int(remain));
}'''

new = '''static void _rmaker_update(void *priv)
{
    user_record_t *u = find_user_by_id(current_user_id);
    int uid = (current_user_id == FP_UNKNOWN) ? 0 : current_user_id;
    if (user_id_param) esp_rmaker_param_update_and_report(user_id_param, esp_rmaker_int(uid));
    if (last_vol_param) esp_rmaker_param_update_and_report(last_vol_param, esp_rmaker_int(last_dispensed_volume));
    if (count_param) esp_rmaker_param_update_and_report(count_param, esp_rmaker_int(u ? u->use_count : 0));
    int daily = u ? u->daily_water : 0;
    if (daily_param) esp_rmaker_param_update_and_report(daily_param, esp_rmaker_int(daily));
    int target = calculate_daily_target(u);
    int remain = target - daily;
    if (remain < 0) remain = 0;
    if (remain_param) esp_rmaker_param_update_and_report(remain_param, esp_rmaker_int(remain));
}

static void update_all_params(void)
{
    esp_rmaker_work_queue_add_task(_rmaker_update, NULL);
}'''

if old in content:
    content = content.replace(old, new)
    with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("DONE")
else:
    print("NOT FOUND")
    idx = content.find('static void update_all_params(void)')
    if idx > 0:
        print(repr(content[idx:idx+450]))
