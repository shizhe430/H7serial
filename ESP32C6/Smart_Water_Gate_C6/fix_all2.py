with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Restore dispense_worker - put back the original data processing
old_dw = '''                /* Data already saved by sim/STM32 handler - just push MQTT update */
                update_all_params();'''

new_dw = '''                user_record_t *u = find_user_by_id(current_user_id);
                water_valve_open = false;
                water_volume += ev.vol;
                last_dispensed_volume = ev.vol;
                if (u) u->use_count++;
                notify_daily_water(ev.vol);'''

content = content.replace(old_dw, new_dw)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE - restored dispense_worker")
