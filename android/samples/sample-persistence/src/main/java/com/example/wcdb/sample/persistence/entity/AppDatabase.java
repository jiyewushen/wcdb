package com.example.wcdb.sample.persistence.entity;

import androidx.room.Database;
import androidx.room.RoomDatabase;

/**
 * Created by johnwhe on 2017/7/12.
 */

@Database(entities = {User.class}, version = 1,exportSchema = false)
public abstract class AppDatabase extends RoomDatabase {
    public abstract UserDao userDao();
}
