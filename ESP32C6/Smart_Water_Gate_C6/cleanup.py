with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Remove the "获取建议" block from write_cb
old = '''        esp_rmaker_param_update_and_report(param, val);
    } else if (strcmp(esp_rmaker_param_get_name(param), "\\xe8\\x8e\\xb7\\xe5\\x8f\\x96\\xe5\\xbb\\xba\\xe8\\xae\\xae") == 0) {
        auto_suggest_enabled = val.val.b;
        if (val.val.b) {
            user_record_t *u = find_user_by_id(current_user_id);
            int daily = u ? u->daily_water : 0;
            int target = calculate_daily_target(u);
            char uname[32] = "\\xe8\\xae\\xbf\\xe5\\xae\\xa2";
            if (u && strlen(u->name) > 0) snprintf(uname, sizeof(uname), "%s", u->name);
            else if (u) snprintf(uname, sizeof(uname), "\\xe7\\x94\\xa8\\xe6\\x88\\xb7%d", u->id);
            snprintf(suggest_question, sizeof(suggest_question),
                     "User %s %dyo %dcm %dkg drank %d/%dml. Reply only 5-10 chars advice.",
                     uname, u ? u->age : 0,
                     (int)(u ? u->height_cm : 0), (int)(u ? u->weight_kg : 0),
                     daily, target);
            xSemaphoreGive(suggest_sem);
        }
        esp_rmaker_param_update_and_report(param, val);
    }
    return ESP_OK;
}'''

new = '''        esp_rmaker_param_update_and_report(param, val);
    }
    return ESP_OK;
}'''

content = content.replace(old, new)
print(f"Removed write_cb block: {'YES' if old in content else 'DONE'}")

# Remove the suggest worker task creation and suggest_sem from app_main
old2 = '''    /* Create suggestion worker task */
    suggest_sem = xSemaphoreCreateBinary();
    if (suggest_sem) {
        xTaskCreate(suggest_worker_task, "suggest", 16384, NULL, 5, NULL);
    }

    /* Create dispense worker queue + task */'''
new2 = '''    /* Create dispense worker queue + task */'''

# Check if old2 exists
if old2 in content:
    content = content.replace(old2, new2)
    print("Removed suggest worker creation")
else:
    print("Suggest worker creation pattern not found")

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
