# Generated by Django 4.1.2 on 2022-11-27 00:23

import django.core.validators
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('index', '0002_alter_crop_crop_year_alter_harvest_farmer'),
    ]

    operations = [
        migrations.AlterField(
            model_name='crop',
            name='amount',
            field=models.PositiveIntegerField(),
        ),
        migrations.AlterField(
            model_name='crop',
            name='crop_year',
            field=models.PositiveIntegerField(null=True),
        ),
        migrations.AlterField(
            model_name='harvest',
            name='current_occupancy',
            field=models.PositiveIntegerField(default=0),
        ),
        migrations.AlterField(
            model_name='harvest',
            name='max_occupancy',
            field=models.PositiveIntegerField(default=0),
        ),
        migrations.AlterField(
            model_name='order',
            name='amount',
            field=models.PositiveIntegerField(),
        ),
        migrations.AlterField(
            model_name='order',
            name='total_price',
            field=models.FloatField(verbose_name=django.core.validators.MinValueValidator(0.0)),
        ),
        migrations.AlterField(
            model_name='review',
            name='stars',
            field=models.PositiveIntegerField(verbose_name=django.core.validators.MaxValueValidator(5)),
        ),
    ]
