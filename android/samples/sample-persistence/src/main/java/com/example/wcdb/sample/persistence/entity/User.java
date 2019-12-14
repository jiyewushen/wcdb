package com.example.wcdb.sample.persistence.entity;

import androidx.room.Entity;
import androidx.room.PrimaryKey;

/**
 * Created by johnwhe on 2017/7/12.
 */

@Entity
public class User {
    @PrimaryKey(autoGenerate = true)
    public int userId;

    public String firstName;
    public String lastName;
}
